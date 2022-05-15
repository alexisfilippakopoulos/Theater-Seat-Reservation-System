#pragma once
#include <stdio.h>

//Constants

const int Ntel = 3;		//number of phone operators
const int Ncash = 2;	//number of cashiers
const int Nseat = 10;	//number of seats per row
const int NzoneA = 10;	//number of rows in the A zone
const int NzoneB = 20;	//number of rows in the B zone
const float PzoneA = 30 / 100; //propability of choosing zone A
const float PzoneB = 1 - PzoneA;//propability of choosing zone 