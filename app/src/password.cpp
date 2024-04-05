#include "password.hpp"
#include <fstream>
#include <iostream>

static const std::string filePath = "password.txt";
static const int defaultLetterShift = 3;
static const int defaultNumberShift = 1;
static const int numLetters = 26;
static const int numDigits = 10;

// simple caesar shift encryption, shift amount changes on loop iteration
std::string Password::encryptPassword(const std::string& password) {
    int callCount = 0;
    std::string encryptedPassword = password;

    for (char& c : encryptedPassword) {
        int letterShift = defaultLetterShift + (callCount % 2);
        int numberShift = defaultNumberShift + (callCount % 2);

        if (std::isdigit(c)) {
            c = '0' + (c - '0' + numberShift) % numDigits;
        } else if (std::isalpha(c)) {
            if (std::islower(c)) {
                c = 'a' + (c - 'a' + letterShift) % numLetters;
            } else {
                c = 'A' + (c - 'A' + letterShift) % numLetters;
            }
        }
        callCount++;
    }

    return encryptedPassword;
}

void Password::writePasswordToFile(const std::string& encryptedPassword) {
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << encryptedPassword;
        file.close();
    } else {
        std::cerr << "Unable to open password file" << std::endl;
    }
}

std::string Password::readPasswordFromFile() {
    std::ifstream file(filePath);
    std::string encryptedPassword;
    if (file.is_open()) {
        std::getline(file, encryptedPassword);
        file.close();
    }
    return encryptedPassword;
}

void Password::savePassword(const std::string& password) {
    std::string encryptedPassword = encryptPassword(password);
    writePasswordToFile(encryptedPassword);
    isPasswordSet = true;
}

bool Password::changePassword(const std::string& oldPassword, const std::string& newPassword) {
    if (!isPasswordSet) {
        std::cerr << "Password not set" << std::endl;
        return false;
    }
    if (!(readPasswordFromFile() == encryptPassword(oldPassword))) {
        std::cerr << "Old password incorrect" << std::endl;
        return false;
    }
    savePassword(newPassword);
    return true;
}

void Password::removePassword() {
    std::remove(filePath.c_str());
    isPasswordSet = false;
}