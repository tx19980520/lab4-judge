#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>  
#include <stdlib.h> 
#include <fstream>

using namespace std;

const string traceFolder = "./trace/";
const string defaultStudentBasic = "lab2.exe";
#if __x86_64__
const string defaultStanderBasic = "lab2_demo_x86.exe";
#else
const string defaultStanderBasic = "lab2_demo_x86.exe";
#endif



const int traceCount = 100;
const string traces[traceCount] = {
  "trace00.txt", "trace01.txt", "trace02.txt", "trace03.txt", "trace04.txt", "trace05.txt", "trace06.txt", "trace07.txt", "trace08.txt", "trace09.txt", 
  "trace10.txt", "trace11.txt", "trace12.txt", "trace13.txt", "trace14.txt", "trace15.txt", "trace16.txt", "trace17.txt", "trace18.txt", "trace19.txt", 
  "trace20.txt", "trace21.txt", "trace22.txt", "trace23.txt", "trace24.txt", "trace25.txt", "trace26.txt", "trace27.txt", "trace28.txt", "trace29.txt", 
  "trace30.txt", "trace31.txt", "trace32.txt", "trace33.txt", "trace34.txt", "trace35.txt", "trace36.txt", "trace37.txt", "trace38.txt", "trace39.txt", 
  "trace40.txt", "trace41.txt", "trace42.txt", "trace43.txt", "trace44.txt", "trace45.txt", "trace46.txt", "trace47.txt", "trace48.txt", "trace49.txt", 
  "trace50.txt", "trace51.txt", "trace52.txt", "trace53.txt", "trace54.txt", "trace55.txt", "trace56.txt", "trace57.txt", "trace58.txt", "trace59.txt", 
  "trace60.txt", "trace61.txt", "trace62.txt", "trace63.txt", "trace64.txt", "trace65.txt", "trace66.txt", "trace67.txt", "trace68.txt", "trace69.txt", 
  "trace70.txt", "trace71.txt", "trace72.txt", "trace73.txt", "trace74.txt", "trace75.txt", "trace76.txt", "trace77.txt", "trace78.txt", "trace79.txt", 
  "trace80.txt", "trace81.txt", "trace82.txt", "trace83.txt", "trace84.txt", "trace85.txt", "trace86.txt", "trace87.txt", "trace88.txt", "trace89.txt", 
  "trace90.txt", "trace91.txt", "trace92.txt", "trace93.txt", "trace94.txt", "trace95.txt", "trace96.txt", "trace97.txt", "trace98.txt", "trace99.txt", 
};

string studentBasic = "";
string standerBasic = "";
string traceFile = "";
int runTraces = traceCount, currentTrace = 0;
bool silent = false, firstFail = false, hideError = false, useColor = true;

int correct = 0, wrong = 0, total = 0;

void useage(const char* progname) {
  cout
    << progname << " [-h] [-e <your_exec>] [-s <stander_exec>] [-t <trace_file>] [-f] [-m] [-q] [-c]" << endl
    << "    -h  Show this message and quit" << endl
    << "    -e  Specify your executable file, default value: " << defaultStudentBasic << endl
    << "    -s  Specify demo executable file, default value: " << defaultStanderBasic << endl
    << "    -t  Run specified trace file" << endl
    << "    -f  Stop at first failed test" << endl
    << "    -m  Hide error message" << endl
    << "    -q  Show final score only, cannot use with -t or -f, include -m" << endl

  ;
  exit(1);
}

string color(string ce) {
  if (useColor) return ce; return "";
}

void parseArguments(int argc, char** argv) {
  int c;
  opterr = 0;
  while ((c = getopt (argc, argv, "e:s:t:fmqch")) != -1) {
    switch (c)
    {
      case 'e': if (studentBasic.size()) useage(argv[0]); studentBasic = optarg; break;
      case 's': if (standerBasic.size()) useage(argv[0]);standerBasic = optarg; break;
      case 't': if (traceFile.size()) useage(argv[0]); traceFile = optarg; break;
      case 'f': if (firstFail) useage(argv[0]); firstFail = true; break;
      case 'm': if (hideError) useage(argv[0]); hideError = true; break;
      case 'q': if (silent) useage(argv[0]); silent = true; break;
      case 'h': useage(argv[0]); break;
      default: useage(argv[0]); break;
    }
  }
  if (silent && (traceFile.size() || firstFail)) useage(argv[0]);
  if (silent) hideError = true;
  if (studentBasic.size() == 0) studentBasic = defaultStudentBasic;
  if (standerBasic.size() == 0) standerBasic = defaultStanderBasic;
}

void clearTempFiles() {
  int r;
  r = system("cat /dev/null > standard.txt");(void)r;
  r = system("cat /dev/null > student.txt");(void)r;
  r = system("cat /dev/null > diff.txt");(void)r;
}

int testTrace(const char* trace) {
  clearTempFiles();
  int r1 = system((string()+studentBasic+" <" + trace + "> ./student.txt").c_str());
  if(r1 != 0) return 2;
  int r2 = system((string()+standerBasic+" < " + trace + " > ./standard.txt").c_str());
  if(r2 != 0)return 1;
  system("diff student.txt standard.txt > diff.txt");
  ifstream is;
  is.open ("diff.txt", ios_base::binary );

  is.seekg (0, ios_base::end);
  int length = is.tellg(); // length 为0则文件为空
  if (length != 0) return 4;
  //clearTempFiles();
  return 0;
}

void runTest(const string currentTrace) {
  if (!silent)  cout << "Trace \"" << currentTrace << "\" ... "; cout.flush();
  int error = testTrace(currentTrace.c_str());
  total++;
  if (!error) {
    if (!silent) cout << color("\x1b[32;1m") << "Pass" << color("\x1b[0m") << endl;
    correct++;
	cout << "Demo output: " << endl << color("\x1b[36m"); cout.flush();
          int r1 = system((string()+"cat standard.txt").c_str()); (void)r1;
          cout << color("\x1b[0m") << endl;
          cout << "Your output: " << endl << color("\x1b[33m"); cout.flush();
          int r2 = system("cat student.txt"); (void)r2;
          cout << color("\x1b[0m") << endl;
  } else {
    wrong++;
    if (!silent) {
      cout << color("\x1b[31;1m") << "Fail" << color("\x1b[0m") << endl;
      if (!hideError) {
        cout << "Trace file: " << endl << color("\x1b[35m"); cout.flush();
        int r0 = system((string() + "cat " + currentTrace).c_str()); (void)r0;
        cout << color("\x1b[0m") << endl;
        if (error == 1) 
		{
			cout << color("\x1b[31m") << "Error occurred while running demo program" << color("\x1b[0m") << endl;
			cout << "Demo output: " << endl << color("\x1b[36m"); cout.flush();
			int r1 = system((string()+"cat standard.txt").c_str()); (void)r1;
			cout << color("\x1b[0m") << endl;
			cout << "Your output: " << endl << color("\x1b[33m"); cout.flush();
			int r2 = system("cat student.txt"); (void)r2;
			cout << color("\x1b[0m") << endl;
		}
        if (error == 2) {
			cout << color("\x1b[31m") << "Error occurred while running your program" << color("\x1b[0m") << endl;
			cout << "Demo output: " << endl << color("\x1b[36m"); cout.flush();
			int r1 = system((string()+"cat standard.txt").c_str()); (void)r1;
			cout << color("\x1b[0m") << endl;
			cout << "Your output: " << endl << color("\x1b[33m"); cout.flush();
			int r2 = system("cat student.txt"); (void)r2;
			cout << color("\x1b[0m") << endl;
		}
        if (error == 4) {
          cout << "Demo output: " << endl << color("\x1b[36m"); cout.flush();
          int r1 = system((string()+"cat standard.txt").c_str()); (void)r1;
          cout << color("\x1b[0m") << endl;
          cout << "Your output: " << endl << color("\x1b[33m"); cout.flush();
          int r2 = system("cat student.txt"); (void)r2;
          cout << color("\x1b[0m") << endl;
        }
      }
    }
    //clearTempFiles();
    if (firstFail) throw exception();
  }
}

void showScore() {
  int score = correct / 5 * 5;
  if (!silent)
    cout << correct << " / " << total << " trace(s) passed." << endl;
  if (total != traceCount) return;
  cout << "Final Score: " << (score / 10) << "." << (score % 10) << endl;
}

int main(int argc, char** argv) {
  parseArguments(argc, argv);
  try {
    if (traceFile.size()) runTest(traceFile);
    else {
      int i = 0;
      for (; i < traceCount; i++) runTest(traceFolder + traces[i]);
    }
  } catch (...) {}
  showScore();
  return 0;
}
