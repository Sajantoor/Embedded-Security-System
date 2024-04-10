/*
Handle saving, changing, and comparing passwords.
*/

#ifndef _PASSWORD_HPP_
#define _PASSWORD_HPP_

#include <string>

class Password {
  private:
    bool isPasswordSet = false;
    std::string password = "";

    std::string encryptPassword(const std::string& password);

    void writePasswordToFile(const std::string& encryptedPassword);

    std::string readPasswordFromFile();

    std::string getPassword();

  public:
    Password(void);

    void savePassword(const std::string& password);

    // Updates password. Requires old password. Returns true if successful.
    bool changePassword(const std::string& oldPassword, const std::string& newPassword);

    void removePassword();

    bool doesPasswordExist();

    bool isPasswordCorrect(const std::string& password);
};

#endif
