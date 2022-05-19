//
// Created by Denis Shevchenko on 25.04.2022.
//

#ifndef MIXERCORE_CCONFIG_H
#define MIXERCORE_CCONFIG_H

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */




class CConfig {
private:
    static inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

// trim from end (in place)
    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

// trim from both ends (in place)
    static inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }
       
    static void _HTTPgetRequest(std::string target);
public: CConfig();
    static int GetConfig(const char *name, std::string &str);
    static int getGlobalValues();

    static int MAX_FACES;
    static int WIDTH;
    static int HEIGHT;
    static std::string RTMP_MAIN;
    static int FRAMERATE;
    static int MIXER_BITRATE;
    static int MIXER_GOP;
    static int HTTP_SERVER_PORT;
    static std::string CONTROL_SERVER;
    static int CONTROL_SERVER_PORT;

    template <typename T>
    static void log(T t)
    {
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::cout << BLUE << t << " <<-" << std::ctime(&end_time)<< RESET;//<< std::endl ;
    }
    template<typename T, typename... Args>
    static void log(T t, Args... args) // recursive variadic function
    {
        std::cout << t << " " ;
        log(args...) ;
    }

    template <typename T>
    static void error(T t)
    {
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::cout << t << " <<-"<<BLUE <<  std::ctime(&end_time) << RESET;//<< std::endl ;
    }
    template<typename T, typename... Args>
    static void error(T t, Args... args) // recursive variadic function
    {
        std::cout << RED << t << " " ;
        error(args...) ;
    }

    static void notifyControl(std::string action, std::string eventid, std::string *param1= nullptr, std::string *param2= nullptr, std::string *param3= nullptr);


};


#endif //MIXERCORE_CCONFIG_H
