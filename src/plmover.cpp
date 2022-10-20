#include <iostream>
#include <sys/stat.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <unistd.h>
#include <md5.hpp>

#include <config.hpp>

std::string basePath = config::rawDict()["remote_pl_dir"];

namespace fs = std::filesystem;
namespace plmover {

bool fileExists(std::string fileUrl){
    struct stat buffer;
    return (stat (fileUrl.c_str(), &buffer) ==0);
}

std::string getCheckSum(std::string fileUrl){
    std::ifstream inFile;
    inFile.open(fileUrl, std::ifstream::binary);

    inFile.seekg(0, std::ios::end);
    long length = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    char* inFileData = new char[length];
    inFile.read(inFileData, length);

    std::string Temp = md5(inFileData);

    return Temp.c_str();
}

/*
int mains(int argc, char *argv[])
{
    //Start opening your file
    ifstream inBigArrayfile;
    inBigArrayfile.open ("Data.dat", std::ios::binary | std::ios::in);

    //Find length of file
    inBigArrayfile.seekg (0, std::ios::end);
    long Length = inBigArrayfile.tellg();
    inBigArrayfile.seekg (0, std::ios::beg);

    //read in the data from your file
    char * InFileData = new char[Length];
    inBigArrayfile.read(InFileData,Length);

    //Calculate MD5 hash
    std::string Temp =  md5(InFileData,Length);
    cout << Temp.c_str() << endl;

    //Clean up
    delete [] InFileData;

    return 0;
}
*/

void checkLocalFile(std::string localUrl){
    std::cout<<"checkLocalFile(localUrl="<<localUrl<<")"<<std::endl;
    if (!fileExists(localUrl)){
        std::cout<<"Local payload file does not exist. Exiting..."<<std::endl;
        exit(1);
    }
}

void checkRemoteFile(std::string remoteUrl){
    if (fileExists(remoteUrl)){
        std::cout<<"Remote payload file already exists. Exiting..."<<std::endl;
        exit(1);
    }
}

std::string getRemoteUrl(std::string globalTag, std::string payloadType,
                         int majorIovStart, int minorIovStart){
    std::string newPath = basePath;
    newPath += globalTag + "/";
    newPath += payloadType + "/";
    newPath += std::to_string(majorIovStart) + "_";
    newPath += std::to_string(minorIovStart);
    newPath += ".dat";
    return newPath;
}

void prepareDirectory(std::string dirName){
    if (!fs::is_directory(dirName) || !fs::exists(dirName)) {
        fs::create_directory(dirName);
    }
}

void prepareDirectories(std::string globalTag, std::string payloadType){
    prepareDirectory(basePath + "/" + globalTag);
    prepareDirectory(basePath + "/" + globalTag + "/" + payloadType);
}

void uploadFile(std::string localUrl, std::string globalTag, std::string payloadType,
                int majorIovStart, int minorIovStart){
    std::cout<<"uploadFile(localUrl="<<localUrl<<")"<<std::endl;
    std::string remoteUrl = getRemoteUrl(globalTag, payloadType, majorIovStart, minorIovStart);
    checkLocalFile(localUrl);
    checkRemoteFile(remoteUrl);
    prepareDirectories(globalTag, payloadType);
    std::filesystem::copy_file(localUrl, remoteUrl);
}

}