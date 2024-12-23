#if !defined(EDITORCOMMAND_H)
#define EDITORCOMMAND_H
#include <GObject.h>
#include <iostream>
struct CommandObj {
    float arg[10];
    std::function<void()> func;
    static std::queue<std::function<void()>> histiryCmd;
};
inline std::queue<std::function<void()>> CommandObj::histiryCmd;
class editorCommand {

  private:
    /* data */
  public:
    static editorCommand edc;
    std::map<std::string, std::function<void()>> command;
    std::string commandStr;
    std::string input[10];

    float fArg[10] = {0};
    void parseString(const std::string &str);
    void executeCommand();
    bool bRun = true;
    editorCommand(/* args */);
    ~editorCommand();
    void loop();

  private:
    void resetInput();
};
inline editorCommand editorCommand::edc;
inline void editorCommand::resetInput() {
    for (auto x : input) {
        x.clear();
    }
}
inline void editorCommand::parseString(const std::string &str) {
    resetInput();
    size_t pos = 0;
    size_t newpos = 0;
    int i = 0;
    while (true) {

        newpos = str.find(' ', pos);
        if (newpos == std::string::npos) {
            input[i] = str.substr(pos);
            break;
        }

        input[i] = str.substr(pos, newpos - pos);
        pos = newpos + 1;
        i++;
    }
}

inline void editorCommand::executeCommand() {
    if (input[0] == "quit") {
        // getWindow()->close();
        bRun = false;
    }
    if (command.find(input[0]) == command.end()) {
        printf("invalid command\n");
    } else
        command[input[0]]();
}

inline editorCommand::editorCommand(/* args */) {}

inline editorCommand::~editorCommand() {}

inline void editorCommand::loop() {
    while (bRun) {
        std::string in;
        std::getline(std::cin, in);
        parseString(in);
        executeCommand();
#if (0)
        // debug
        for (auto x : input)
            std::cout << x;
#endif
    }
}

#endif // EDITORCOMMAND_H
