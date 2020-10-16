/***********************************************************************/
/*   SEMANTIC KERNEL                                                   */
/*                                                                     */
/***********************************************************************/
#include <fstream>
#include <string>
#include <iostream>
//#include<string.h>
#include<stdlib.h>
#include<math.h>
#include "svm_common.h"

using namespace std;
int docnum_null_in_advanced_kernels = 0; //count for docnum=-1

//define matrix
long matrix_size = 0;
string* pMatrixWordID = NULL;
double **pMatrix = NULL;

int currentDocNumber = 0;
void free_matrix() {
    for (int i = 0; i < matrix_size+1; i++)
    {
        delete[]  pMatrix[i];
    }
    delete[] pMatrixWordID;
}

void load_matrix() {
//load into memory
/* project.out.step5.matrix.txt
    MATRIX_SIZE=4805
    WORD_ID=1 conceptID=C0034703 location=B01.050.050.199.520.760.200
    1.0
    0.380952
    0.125
    ...
    WORD_ID=4804 conceptID=C3494179 location=G04.366.500
    1.0
    0.166667
    WORD_ID=4805 conceptID=C0031860 location=D02.455.849.291.523
    1.0
    total words: 4805 / 4805 processed

    project.out.step6.txt
1 	|BT| (NP (NP (NP (NP (JJ 2-Amino-3,8-dimethylimidazo) (-LRB- -LSB-) (JJ 4,5-f) (-RRB- -RSB-) (NN quinoxaline)) (PRN (-LRB- -LRB-) (NP (NN MeIQx)) (-RRB- -RRB-))) (SBAR (S (NP (JJ potent)
        (NN mutagen)) (VP (VBZ induces) (NP (NP (NN tumor) (NN liver)) (, ,) (NP (NP (NN lung) (JJ hematopoietic) (NN system) (NN CDF1) (NN mouse) (NN liver)) (, ,) (NP (NNP Zymbal) (NN gland) (NN skin)
        (NN F344) (NN rat))))))))) |ET| |BS|1:B01.050.050.199.520.760.200 2:B01.050.150.900.649.313.992.635.505.500 3:D01.248.497.158.380 4:D27.888.569.468 5:C04.588.274.623 6:A03.620 7:A04.411 8:A15.378 9:A17.815|ES|
        1:1 2:1 3:1 4:1 5:1 6:1 7:1 8:1 9:1 10:1 11:1 12:1 13:1 14:1 15:1 16:1 17:1 18:1 19:1 13:1 14:1 20:1 21:1 22:1 23:1 24:1 25:1
2 	|BT| (NP (NP (NN P53) (NN gene) (JJ deficient) (NN mouse)) (SBAR (-LRB- -LRB-) (S (S (NP (NN p53) (PRN (: -) (: /) (: -) (-RRB- -RRB-))) (VP (VBD found) (ADVP (RB spontaneously)) (VP (VB develop)
        (NP (JJ malignant) (NN lymphoma) (NN hemangiosarcoma))))) (, ,) (IN whereas) (FRAG (NP (NN heterozygote)) (-LRB- -LRB-) (NP (NP (NP (NN p53)) (CC +) (: /) (: -) (-RRB- -RRB-) (NN mouse) (NN display) (JJ high)
        (NN incidence) (NN tumor) (JJ urinary) (NN bladder)) (VP (VBN treated) (NP (NN N-butyl-N) (: -) (-LRB- -LRB-) (NN 4-hydroxybutyl) (-RRB- -RRB-) (NN nitrosamine)))))))) |ET|
        |BS|10:G05.360.340.024.340.375.249.385 2:B01.050.150.900.649.313.992.635.505.500 11:E05.318.308.985.525.375 12:C04.557.450.795.390 13:Z01.058.290.190.800 14:D08.811.520.224.800 15:C04.588.945.947.960 16:C04.557.386
        17:G05.380.383 18:D02.654.442 19:E02|ES| 26:1 27:1 28:1 19:1 6:1 29:1 8:1 30:1 31:1 32:1 33:1 34:1 35:1 14:1 36:1 37:1 6:1 38:1 8:1 19:1 39:1 40:1 41:1 12:1 42:1 43:1 44:1 45:1 6:1 46:1 8:1 47:1 25:1
1 	|BT| (NP (NP (NN P53) (NN gene) (JJ deficient) (NN mouse)) (SBAR (-LRB- -LRB-) (S (S (NP (NN p53) (PRN (: -) (: /) (: -) (-RRB- -RRB-))) (VP (VBD found) (ADVP (RB spontaneously)) (VP (VB develop) (NP (JJ malignant) (NN lymphoma)
        (NN hemangiosarcoma))))) (, ,) (IN whereas) (FRAG (NP (NN heterozygote)) (-LRB- -LRB-) (NP (NP (NP (NN p53)) (CC +) (: /) (: -) (-RRB- -RRB-) (NN mouse) (NN display) (JJ high) (NN incidence) (NN tumor) (JJ urinary) (NN bladder))
        (VP (VBN treated) (NP (NN N-butyl-N) (: -) (-LRB- -LRB-) (NN 4-hydroxybutyl) (-RRB- -RRB-) (NN nitrosamine)))))))) |ET| |BS|10:G05.360.340.024.340.375.249.385 2:B01.050.150.900.649.313.992.635.505.500 11:E05.318.308.985.525.375
        12:C04.557.450.795.390 13:Z01.058.290.190.800 14:D08.811.520.224.800 15:C04.588.945.947.960 16:C04.557.386 17:G05.380.383 18:D02.654.442 19:E02|ES| 26:1 27:1 28:1 19:1 6:1 29:1 8:1 30:1 31:1 32:1 33:1 34:1 35:1 14:1 36:1 37:1
        6:1 38:1 8:1 19:1 39:1 40:1 41:1 12:1 42:1 43:1 44:1 45:1 6:1 46:1 8:1 47:1 25:1
*/
    string strMatrixFile = "project.out.step6.matrix.txt";
    ifstream fileMatrix(strMatrixFile);
//    ofstream out("semanticKernel.out.txt");
    cout << "load_matrix from file="+strMatrixFile << endl;
    string line;
    long idxLine = 0;
    long totalLines = 0;                   // number of lines

    if (fileMatrix) // 有该文件
    {
        // read first line
        idxLine = idxLine + 1;
        getline(fileMatrix, line);
        string tmpStr = "MATRIX_SIZE=";
        int pos = line.find(tmpStr);
        if (pos < 0) {
            cout << "cannot find : " + tmpStr+" in file : " + strMatrixFile << endl;
            exit(1);

        }
        string tmpResult = line.substr(pos + tmpStr.length(), line.length() - pos - tmpStr.length());
        cout << tmpStr + " " + tmpResult << endl;
        matrix_size = stol(tmpResult);
    //    cout << tmpStr + " " + tmpResult + " converted to: " + to_string(matrix_size) << endl;
    }
    else // 没有该文件
    {
        cout << "cannot find file: "+ strMatrixFile << endl;
        exit(1);
    }

    //define matrix
    pMatrixWordID = new string[matrix_size+1];
    char*    wordID = NULL;
    pMatrix = new double*[matrix_size+1];
    for (int i = 0; i < matrix_size+1; i++)
    {
        pMatrix[i] = new double[matrix_size+1];
    }
    int idxRaw = 0;
    int idxColumn = 0;
    for (idxRaw = 0; idxRaw < matrix_size; idxRaw++)
    {
        idxLine = idxLine + 1;
        getline(fileMatrix, line);
        pMatrixWordID[idxRaw] = line.substr(line.find("=") + 1, line.find(" ") - line.find("=") - 1);
//        memcpy(&pMatrixWordID[idxRaw], line.c_str(), line.length());
//        cout << "find wordID="+pMatrixWordID[idxRaw] << endl;
        for (idxColumn = 0; idxColumn < idxRaw; idxColumn++)
        {
            pMatrix[idxRaw][idxColumn] = 0.0;
        }
        for (idxColumn = idxRaw; idxColumn < matrix_size; idxColumn++)
        {
            idxLine = idxLine + 1;
            getline(fileMatrix, line);
            pMatrix[idxRaw][idxColumn] = stod(line);
        }
        if (idxLine >= (matrix_size+1) * matrix_size / 2 + matrix_size) // line 1: "MATRIX_SIZE=4805".  line "WORD_ID=": 4805, data: (1+4805)*4805/2
        {
            break;
        }
    }
    cout << "load matrix done, idxLine=" + to_string(idxLine) + "; " + line << endl;
/*    idxLine = 0;
    while (getline(fileMatrix, line)) // line中不包括每行的换行符
    {
        // read every line
        idxLine = idxLine + 1;
//        cout << line << endl;
        //out << line << endl; // 输入到2.txt中
    }*/
}


void calc_with_matrix(SEMANTIC_VECTOR_SET *pVectorSet) {
    // doc * matrix
    /*                       Matrix                      loaded from file
                       [P11, P12, P13, P14, P15]      [P11, P12, P13, P14, P15]     12345  1      1    1    1
                       [P21, P22, P23, P24, P25]      [  0, P22, P23, P24, P25]            2345   2    2    2
    [X1,X2,X3,X4,X5] * [P31, P32, P33, P34, P35] ==>  [  0,   0, P33, P34, P35] ==>               345  3    3
                       [P41, P42, P43, P44, P45]      [  0,   0,   0, P44, P45]                        45   4
                       [P51, P52, P53, P54, P55]      [  0,   0,   0,   0, P55]                             5

    */
    pVectorSet->values = new double[matrix_size+1];
    string strOut = "[";
    char tmpResult[100];
    for (int i = 0; i < matrix_size; i++)
    {
        pVectorSet->values[i] = 0;

        for (int j = 0; j < pVectorSet->num_of_words; j++)
        {
            int iWordID = stoi(pVectorSet->words[j].wordID) -1;//数据中词的ID是从1开始
            if (iWordID < 0) {
                cout << "iWordID < 0 , iWordID=" + to_string(iWordID) << endl;
                iWordID = 0;
            }
            std::string strLocation = pVectorSet->words[j].location;
            // use wordID, and TF-IDF to calculate, so no need for location
            if (strLocation.find("x.") != -1 || strLocation.find("y.") != -1)
            {  // this wordID cannot be used
                continue;
            }
            else
            {
                if (iWordID > i) {
                    pVectorSet->values[i] += pMatrix[i][iWordID] * 1; //currently TF-IDF is not used yet, so use 1 instead
                }
                else
                {
                    pVectorSet->values[i] += pMatrix[iWordID][i] * 1;
                }
            }
        }
/*        if (currentDocNumber == 1) {
            sprintf(tmpResult, "(%04d:%0.6f), ", i + 1, pVectorSet->values[i]);
            strOut += tmpResult;
        } */
    }
 /*   if (currentDocNumber == 1) {
        cout << "calc_matrix:" + strOut + "]" << endl;
        cout << "matrix [-4][-1]:" + to_string(pMatrix[matrix_size - 5][matrix_size - 2]) << endl;
        cout << "matrix [-3][-1]:" + to_string(pMatrix[matrix_size - 4][matrix_size - 2]) << endl;
        cout << "matrix [-2][-1]:" + to_string(pMatrix[matrix_size - 3][matrix_size - 2]) << endl;
        cout << "matrix [-1][-1]:" + to_string(pMatrix[matrix_size - 2][matrix_size - 2]) << endl;
        cout << "matrix [][-1]:" + to_string(pMatrix[matrix_size - 1][matrix_size - 2]) << endl;
        cout << "matrix [-4][]:" + to_string(pMatrix[matrix_size - 5][matrix_size - 1]) << endl;
        cout << "matrix [-3][]:" + to_string(pMatrix[matrix_size - 4][matrix_size - 1]) << endl;
        cout << "matrix [-2][]:" + to_string(pMatrix[matrix_size - 3][matrix_size - 1]) << endl;
        cout << "matrix [-1][]:" + to_string(pMatrix[matrix_size - 2][matrix_size - 1]) << endl;
        cout << "matrix [][]:" + to_string(pMatrix[matrix_size - 1][matrix_size - 1]) << endl;
        cout << "matrix [-1][-4]:" + to_string(pMatrix[matrix_size - 2][matrix_size - 5]) << endl;
        cout << "matrix [-1][-3]:" + to_string(pMatrix[matrix_size - 2][matrix_size - 4]) << endl;
        cout << "matrix [-1][-2]:" + to_string(pMatrix[matrix_size - 2][matrix_size - 3]) << endl;
        cout << "matrix [-1][-1]:" + to_string(pMatrix[matrix_size - 2][matrix_size - 2]) << endl;
        cout << "matrix [-1][]:" + to_string(pMatrix[matrix_size - 2][matrix_size - 1]) << endl;
        cout << "matrix [][-4]:" + to_string(pMatrix[matrix_size - 1][matrix_size - 5]) << endl;
        cout << "matrix [][-3]:" + to_string(pMatrix[matrix_size - 1][matrix_size - 4]) << endl;
        cout << "matrix [][-2]:" + to_string(pMatrix[matrix_size - 1][matrix_size - 3]) << endl;
        cout << "matrix [][-1]:" + to_string(pMatrix[matrix_size - 1][matrix_size - 2]) << endl;
        cout << "matrix [][]:" + to_string(pMatrix[matrix_size - 1][matrix_size - 1]) << endl;
    }*/
}

double choose_semantic_kernel(KERNEL_PARM * kernel_parm, DOC * a, DOC * b)
{
/*    printf("begin choose_semantic_kernel: %ld x %ld\n", a->docnum, b->docnum);
    if (a->semanticVector != NULL)
    {
        printf("read a finished, num_of_words=%d\n", a->semanticVector->num_of_words); fflush(stdout);
        for (int j = 0; j < a->semanticVector->num_of_words; j++) {
            printf("ID=%d, len=%d, wordID=%s, len=%d, location=%s\n", j + 1, strlen(a->semanticVector->words[j].wordID), a->semanticVector->words[j].wordID,
                strlen(a->semanticVector->words[j].location), a->semanticVector->words[j].location);
        }
    }
    if (b->semanticVector != NULL)
    {
        printf("read b finished, num_of_words=%d\n", b->semanticVector->num_of_words); fflush(stdout);
        for (int j = 0; j < b->semanticVector->num_of_words; j++) {
            printf("ID=%d, len=%d, wordID=%s, len=%d, location=%s\n", j + 1, strlen(b->semanticVector->words[j].wordID), b->semanticVector->words[j].wordID,
                strlen(b->semanticVector->words[j].location), b->semanticVector->words[j].location);
        }
    }
*/
    double result=0,resulta=0, resultb=0, resultc = 0;
    if (a->semanticVector != NULL && b->semanticVector != NULL) {
        cout << "choose_semantic_kernel(): a->semanticVector is not NULL" << endl;
   //     if (a->semanticVector->num_of_words > 0 && b->semanticVector->num_of_words > 0) {
            cout << "choose_semantic_kernel(): a->semanticVector->num_of_words="+to_string(a->semanticVector->num_of_words) << endl;
            cout << "choose_semantic_kernel(): b->semanticVector->num_of_words=" + to_string(b->semanticVector->num_of_words) << endl;
            for (int i = 0; i < matrix_size; i++)
            {
                if (i == 0) {
                    cout << "choose_semantic_kernel(): a->semanticVector->values  i=" + to_string(i) << endl;
                }
                result += (a->semanticVector->values[i]) * (b->semanticVector->values[i]);
                resulta += (a->semanticVector->values[i]) * (a->semanticVector->values[i]);
                resultb += (b->semanticVector->values[i]) * (b->semanticVector->values[i]);
            }
     //   }
    }
/*    if (a->docnum == -1 || b->docnum == -1) {
        printf("choose_semantic_kernel: %ld x %ld, result=%lf\n", a->docnum, b->docnum, result);
    }*/
/*    double resultd = result * result;
    double resulte = resulta * resultb;
    cout << "result2=" + to_string(result * result) + "; resulta*resultb=" + to_string(resulta * resultb) << endl;
    */
    resultc = sqrt(resulta*resultb);
    if (resulta != 0 && resultb != 0) {
        resultc = sqrt(resulta*resultb);
    }
    else {
        result = 0;
    }
    static double resultmin = 100000;
    static double resultmax = result;
    if (a->docnum != -1 && b->docnum != -1) {
        if (resultmin > result) {
            resultmin = result;
            cout << "resultmin=" + to_string(resultmin) << endl;
        }
        if (resultmax < result) {
            resultmax = result;
            cout << "resultmax=" + to_string(resultmax) << endl;
        }
    }
//    resultmin = 31.528580
//    resultmin = 0.000000
//    resultmax = 11848.925211
//    resultmax = 25393.985104
//    resultmax = 95824.459427---in group 789

//    static double resultmin = 0;
 //   static double resultmax = 100000;
/*    if (result > resultc) {
        cout << "result>resultc, result=" + to_string(result)+ "; resultc=" + to_string(resultc) + "; result/resultc=" + to_string(result/resultc) << endl;
    } */
    if (resultc != 0) {
  //      cout << "semantic_kernel result=" + to_string(result)+ "; resultc=" + to_string(resultc) << endl;
      //  result = (result / resultc-0.9)*10;
       // result = result / resultc;
    }
    else {
        result = 0;
    }
 //   cout << "final result="+to_string(result) << endl;
    return result/10000;
}

// | BS | vectors:location | ES | ==>
// C0206669:C04.557.470.035.120 C0045822:x.x.x.x C0086582:x.x.x C0087111:E02 C0027651:C04 ==> change conceptID to wordID, without x.x.x.x
// 1:C04.557.470.035.120 2:E02 4:C04, wordID in increase order
void read_semantic_vectors(char *line, int len, SEMANTIC_VECTOR_SET *pVectorSet) {

    bool bIsWord = false;
    int wnum = 0, wpos=0;
    int tmpWordBegin = 0, tmpWordEnd = 0, tmpColonPos = 0;
    for (int i = 0; i < len; i++)
    {
        bIsWord = false;
        while ((isspace((int)line[i]) == 0) && (i < len)) {
            bIsWord = true;
            i++;
        }
        if (bIsWord) {
            wnum++;
        }
    }
    // allocate sequencial memory
//    SEMANTIC_WORD *tempWords = (SEMANTIC_WORD *)my_malloc(sizeof(SEMANTIC_WORD) * wnum);
    SEMANTIC_WORD *tempWords = new SEMANTIC_WORD[wnum];

    for (int i = 0; i < len; i++)
    {
        bIsWord = false;
        tmpWordBegin = tmpWordEnd; //begin = last end
        tmpColonPos = tmpWordEnd; //colon = last end
        while ((isspace((int)line[i]) == 0) && (i < len)) {
            if (bIsWord == false)
            {
                tmpWordBegin = i;
                tmpColonPos = i; //to make sure colon is behind begin
            }
            if (line[i] == ':')
            {
                tmpColonPos = i;
            }
            bIsWord = true;
            i++;
        }
        if (bIsWord) {
            //get the word
            tmpWordEnd = i;
            //copy wordID
            tempWords[wpos].wordID = new char[tmpColonPos - tmpWordBegin + 1];
            memcpy(tempWords[wpos].wordID, &line[tmpWordBegin], tmpColonPos - tmpWordBegin);
            tempWords[wpos].wordID[tmpColonPos - tmpWordBegin] = 0;
            //copy location string
            tmpWordBegin = tmpColonPos + 1;
            tempWords[wpos].location = new char[tmpWordEnd - tmpWordBegin+1];
            memcpy(tempWords[wpos].location, &line[tmpWordBegin], tmpWordEnd - tmpWordBegin);
            tempWords[wpos].location[tmpWordEnd - tmpWordBegin] = 0;
            wpos++;
        }
    }
    pVectorSet->words = tempWords;
    pVectorSet->num_of_words = wnum;
    calc_with_matrix(pVectorSet);
}

void read_semantic_vector_set(char *line, DOC *example, int *pos) {

    int j;
    char *V_start=NULL;
    char *V_end=NULL;
//    example->semanticVector = NULL;
    V_start = strstr(line, "|BS|");
    if (V_start == NULL)
    {
        cout << "read_semantic_vector_set(): no |BS| found, line=" << endl;
        printf("%s", line); fflush(stdout);
        return;
    }
    V_start += 4; // bypass |BS|
    // printf("\n\n<%s>\n",V_start);fflush(stdout);
    V_end = strstr(V_start, "|ES|");
    if (V_end == NULL) {
        printf("ERROR: an End Vector |ES| marker is missing (line %d)\n",example->docnum);
        printf("Line Content:>>> %s\n", line);fflush(stdout);
        return;
    }
    //printf("\n-----------------------------------------\nParse Tree: %s\n\n\n",T_start);
    int len = V_end - V_start;
//    printf("read semantic_kernel from line %d ==== %s\n", example->docnum, V_start-4);
    currentDocNumber = example->docnum;
    cout << "read_semantic_vector_set(): line=" << endl;
    printf("%s", line); fflush(stdout);
    cout << "read_semantic_vector_set(): docnum="+to_string(example->docnum) << endl;
    example->semanticVector = (SEMANTIC_VECTOR_SET *)my_malloc(sizeof(SEMANTIC_VECTOR_SET));
    read_semantic_vectors(V_start, len, example->semanticVector);
    cout << "after read_semantic_vector_set(): docnum=" + to_string(example->docnum) << endl;
    *pos += V_end - line + 4;
}




