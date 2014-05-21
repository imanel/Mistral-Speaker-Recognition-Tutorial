#include <iostream>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <map>
#include <assert.h>
#include <algorithm>
#include <unistd.h>

using namespace std;

#define THRESHOLD 2

void trainWorldModel(){


	int key;

	cout << "Enter 1 to CONTINUE or other number to CANCEL: ";

	while(cin >> key){

		if (key != 1)
		 	break;

		string command = "./scripts/01_feature_extraction_trainworld.sh";


		cout << "Training World Model......Please Wait" << endl;
		sleep(3);

		int ret = system(command.c_str());

		if (ret == 0)
		{
			cout << "\n\n\t Done Extracting Features\n" << endl;

			command = "./scripts/02_RUN_spro_front-end.sh trainWorldData";

			ret = system(command.c_str());

			if (ret == 0)
			{
				cout << "Done" << endl;

				command = "./scripts/03_TrainUBM.sh";

				ret = system(command.c_str());

				if (ret == 0)
				{
					cout << "Done Creating UBBM" << endl;
				}else{
					cerr << "Stage 3: Failed (Training UBBM)" << endl;
					exit(-1);
				}
			}else{
				cerr << "Stage 2: Failed" << endl;
				exit(-1);
			}

			break;
		}else{
			cerr << "\n\n\t An error occured while Training World Model\n" << endl;
			exit(-1);
		}

	}


}

string genRandomSequence() {
	//modified the solution at
	//http://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    const int len = 5;
    char temp[len];

  	srand(time(NULL));
    for (int i = 0; i < len; ++i) {
        temp[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    string randed (temp);
	return randed;
}


void commenceTraining(){

	string command = "./scripts/01_feature_extraction_traintarget.sh";

	int ret = system(command.c_str());

	if (ret == 0)
	{
		cout << "Finished Extracting Features" << endl;

		command = "./scripts/02_RUN_spro_front-end.sh trainGenuineData";

		ret = system(command.c_str());

		if (ret == 0)
		{
			cout << "Stage 2 Done" << endl;

				command = "./scripts/04_TrainTargets.sh";

				ret = system(command.c_str());

				if (ret == 0)
				{
					cout << "Done Creating GMM" << endl;
				}else{
					cerr << "Stage 3: Failed (Training GMM)" << endl;
					exit(-1);
				}
		}else{
			cerr << "Stage 2: Failed" << endl;
			exit(-1);
		}

	}else{
		cout << "Extracting Features Failed" << endl;
		exit(-1);
	}
}

void enrolUser(std::vector< std::pair<string, string> > &memDatabase){

	string name, file;
	int key;

	int counter = 0;

	do{
		cout << "NAME OF USER: ";

		std::cin.ignore();

		std::getline (std::cin,name);

		cout << endl;

		cout << "Record is starting in 3 seconds..." << endl;

		sleep(3);

		cout << "Recording...Read this\n\n\tIf I have seen further it is by standing on the shoulders of giants.\n\n" << endl;


		file = genRandomSequence();
	    string filename = file + ".sph";


		//string command = "python ./scripts/record.py ./enrolled/sounds/" + filename;
		string command = "rec -c 1 -r 16000 -e u-law trainGenuineData/sph/" +filename+" trim 0 00:10";
		//string command = "arecord -t wav -r 16000 -d 6 -f S16_LE ./enrolled/sounds/" + filename;

		int ret = system(command.c_str());

		if (ret == 0)
		{
			memDatabase.push_back(std::make_pair(name, file));
			cout << "Registered: " << name << endl;

			/////////////////////////////////////////////////////
			//Use STD LIBRARY HERE later
			char strcounter[10];

			sprintf(strcounter, "spk%d",++counter);

			string command2;


			if (memDatabase.size() == 1)
			{
				command = "echo \"" + file + "\" > trainGenuineData/data.lst";
				command2 = "echo \"" + string(strcounter) + " " + file + "\" > ndx/trainModel.ndx";
			}else{
				command = "echo \"" + file + "\" >> trainGenuineData/data.lst";
				command2 = "echo \"" + string(strcounter) + " " + file + "\" >> ndx/trainModel.ndx";
			}

			assert(system(command.c_str()) == 0);
			assert(system(command2.c_str()) == 0);


			///////////////////////////////////////////////////////


		}else{
			cerr << "Unable to register user: Please make sure SOX is installed. or install as follows: sudo apt-get install sox" << endl;
			exit(-1);
		}

		cout << "Enter 1 to CONTINUE enrolling or other number to TRAIN: ";

	}while((cin >> key) && key == 1);


	int count = 0;

	for(std::vector<std::pair<string, string> >::iterator itt = memDatabase.begin(); itt != memDatabase.end(); itt++){
		count++;
		string command = "echo \"" + itt->second + " ";


		int counter = 0;
		char strcounter[10];

		for(std::vector<std::pair<string, string> >::iterator it = memDatabase.begin(); it != memDatabase.end(); it++)
		{

			// if (itt->second == it->second)
			// 	continue;

			sprintf(strcounter, "spk%d",++counter);
			command += string(strcounter) + " ";
			//command += it->second + " ";
		}

		if (count == 1)
		{
			command += "\" > ndx/computetest_gmm_target-seg.ndx";
		}else{
			command += "\" >> ndx/computetest_gmm_target-seg.ndx";
		}

		//fprintf(stderr, "%s\n", command.c_str());
		//exit(-1);
		assert(system(command.c_str()) == 0);
	}

	commenceTraining();
}


void makeDecizion(std::vector<std::pair<string, string> > &memDatabase){

	// FILE * fp = NULL;
	// fp = freopen("res/imp-seg_gmm.res", "r", stdin);

	// if(fp == NULL){
	// 	cout << "No rezult generated. " << endl;
	// 	exit(-1);
	// }

	// char gender;
	// int binary;
	// string impostor, target;
	// float score;

	// float highestScore;
	// string who;

	// bool firstTime = true;

	// while(cin >> gender){
	// 	cin >> impostor >> binary >> target >> score;

	// 	if (firstTime)
	// 	{
	// 		highestScore = score;
	// 		who = target;
	// 		firstTime = false;

	// 	}else{

	// 		if (highestScore < score)
	// 		{
	// 			highestScore = score;
	// 			who = target;
	// 		}
	// 	}
	// }

	// if (highestScore < THRESHOLD)
	// {
	// 	cout << "\n\n\n\tUser Does not exists, Please Try Authenticating again or adjust the threshold (strictness)\n\n" << endl;
	// 	cerr << "\n\n\n\tHighest Scorer: " << who << " with score: " << highestScore << endl << endl;
	// 	return;
	// }


	// for(std::vector<std::pair<string, string> >::iterator itt = memDatabase.begin(); itt != memDatabase.end(); itt++)
	// {
	// 	if(itt->second == who){

	// 		cout << "\n\n\n\tYour Voice Matches: " << itt->first << " With Score: " << highestScore << endl;
	// 		break;
	// 	}
	// }

}

void voiceAuthentication(std::vector<std::pair<string, string> > &memDatabase){

	string key;

	std::cin.ignore();

	cout << "Press Enter to Verify: ";
	std::getline (std::cin,key);

	cout << endl;

	cout << "Record is starting in 3 seconds..." << endl;

	sleep(3);

	cout << "Recording...Read this\n\n\t This is the victory that overcommet the world, even our faith. - 1 John 5:4.\n\n" << endl;

	string file = genRandomSequence();
	string filename = file + ".sph";

	//string command = "python ./scripts/record.py ./verify/sounds/" + filename;
	//string command = "arecord -t wav -r 16000 -d 6 -f S16_LE ./verify/sounds/" + filename;
	string command = "rec -c 1 -r 16000 -e u-law trainImpostorData/sph/" +filename+" trim 0 00:10";

	int ret = system(command.c_str());

	if (ret == 0)
	{
		cerr << "Done recording..." << endl;

		/////////////////////////////////////////////////////
			//Use STD LIBRARY HERE later
			command = "echo \"" + file + "\" > trainImpostorData/data.lst";
			string command2 = "echo \"" + file + " " + file + "\" > ndx/trainImp.ndx";
			string command3 = "echo \"" + file + "\" > ndx/computetest_gmm_imp-imp.ndx";
//computetest_gmm_imp-imp.ndx
			assert(system(command.c_str()) == 0);
			assert(system(command2.c_str()) == 0);
			assert(system(command3.c_str()) == 0);

			command = "echo \"" + file + "  ";
			string impcommand = "echo \"";

			int counter = 0;
			char strcounter[10];

			for(std::vector<std::pair<string, string> >::iterator it = memDatabase.begin(); it != memDatabase.end(); it++){

				sprintf(strcounter, "spk%d",++counter);
				command += string(strcounter) + "  ";
				impcommand += it->second + "  "+ file + "\n";
			}
			command2 = command;
			command += "\" > ndx/target_imp.ndx";
			command2 += "\" > ndx/computetest_gmm_target-imp.ndx";
			impcommand += "\" > ndx/computetest_gmm_imp-seg.ndx";

			assert(system(command.c_str()) == 0);
			assert(system(command2.c_str()) == 0);
			assert(system(impcommand.c_str()) == 0);

		///////////////////////////////////////////////////////

	int count = 0;

	for(std::vector<std::pair<string, string> >::iterator itt = memDatabase.begin(); itt != memDatabase.end(); itt++){
		count++;

		string command = "echo \"" + itt->second + "  " + file;

		if (count == 1)
		{
			command += "\" > ndx/imp_seg.ndx";
		}else{
			command += "\" >> ndx/imp_seg.ndx";
		}

		//fprintf(stderr, "%s\n", command.c_str());
		//exit(-1);
		assert(system(command.c_str()) == 0);
	}




		///////////////////////////////////////////////////////////////

		cout << "Verification in progress.....please wait" << endl;

		sleep(3);

		command = "./scripts/04_TrainTargetsIpostors.sh";

		ret = system(command.c_str());

		if (ret == 0)
		{
			cout << "Done: " << endl;

			//command = "python ./scripts/mdecide.py ./verify/res/test_all.res";
			//command = "true";
			//command = "./bin/Scoring.exe --mode NIST --inputFile verify/res/test_all.res --outputFile outfile.out --threshold 1 --segTypeTest 1side --trainTypeTest 1side --adaptationMode n";
			command = "./scripts/5_Final_testingAndScoring.sh";

			ret = system(command.c_str());

				if (ret == 0)
				{
					cout << "Verification Done" << endl;
					makeDecizion(memDatabase);
				}else{
					cerr << "Error in Verification" << endl;
					exit(-1);
				}

		}else{
			cerr << "Error in Verification" << endl;
		}


	}else{
		cerr << "Verification Failure: Please make sure SOX is installed. or install as follows: sudo apt-get install sox" << endl;
	}


}


void cleanUpTheMessAndExit(std::vector<std::pair<string, string> > &memDatabase){
	cout << "Good Bye" << endl;
	exit(0);
}


void printMenu(){


	for (int i = 0; i < 50; ++i)
		cout << "-";
	cout << endl;

	cout << "\t\tMENU" << endl;

	for (int i = 0; i < 50; ++i)
		cout << "-";
	cout << endl;

	cout << "1 - Train World Model" << endl;
	cout << "2 - Enrol User Voice" << endl;
	cout << "3 - Voice Authentication/Verification" << endl;
	cout << "4 - Exit" << endl;
}

void initializeProject(){

	string command = "rm trainGenuineData/sph/* trainImpostorData/sph/* res/* log/* 2> /dev/null";
	system(command.c_str());
}


int main(int argc, char const *argv[])
{

	initializeProject();

	std::vector<std::pair<string, string> > memDatabase;

	printMenu();

	int selected;

	while(cin >> selected){

		switch(selected){

			case 1:
					trainWorldModel();
					printMenu();
			break;

			case 2:
					enrolUser(memDatabase);
					printMenu();
			break;

			case 3:
					voiceAuthentication(memDatabase);
					printMenu();
			break;

			case 4:
					cleanUpTheMessAndExit(memDatabase);
			break;
			default:
					cout << "Invalid selection. Please choose from 1 to 4" << endl;
					printMenu();
		}


	}

	return 0;
}
