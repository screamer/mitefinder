//
//  main.cpp
//  miteFinder
//
//  Created by Jialu HU on 16/10/14.
//  Copyright © 2016年 Jialu HU. All rights reserved.
//
#include <iostream>
#include <string>
#include <time.h>
#include "genome.h"
#include "mite.h"
#include "mite_finder.h"
#include "argparser.h"

struct Option{
    std::string inputfilename;
    std::string outputfilename;
    bool enable_mismatch;
    int fragnment_length;
    Option(){
        enable_mismatch=true;
        fragnment_length=10000;
    }
};

int main(int argc, const char * argv[]) {
	double seconds=0;
    ArgParser mf_parser;
    Option mf_option;
    Genome osgenome;
    
    // Parser the argument.
    mf_parser.setName("MiteFinder", "An application for detecting miniature inverted-repeat transposable elements on a genome-wide scale.");
    mf_parser.setVerion("Version 1.0.006");
    mf_parser.refOption("input", "The path of a input file.", mf_option.inputfilename, "", true);
    mf_parser.refOption("fragnment_length", "Length of fragnment. Default is 10000.", mf_option.fragnment_length, 10000);
    //mf_parser.boolOption("enable_mismatch", "Logical. It can enable the detection of mismatch base pairs if 1, otherwise 0. Default is 1.");
    if(!mf_parser.run(argc, argv))
        return 1;
    
    // Read DNA sequences from the input file.
	time_t start_time,end_time;
	time(&start_time);
	osgenome.readSequence(mf_option.inputfilename);
    int numChr=osgenome.getNumChrom();
    Seed_set tset;
    std::fstream output;
    
    // Write MITEs to the output file.
    output.open(mf_option.outputfilename,std::ios_base::out);
    for(int i=0;i<numChr;i++) {
        char* pchr=osgenome.getChrom(i);
		mite_finder(tset,pchr,mf_option.enable_mismatch,mf_option.fragnment_length,MIN_LENGTH_TIR);
        write_seed(tset,pchr,output,i+1);
        tset.clear();
    }
	time(&end_time);
    
    // Show the processing duration.
	seconds= difftime(end_time, start_time);
	std::cout << "The program cost "<<seconds<<" seconds totally to search for MITEs."<<std::endl;
	output.close();
    return 0;
}
