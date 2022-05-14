//
// Created by Denis Shevchenko on 25.04.2022.
//

#ifndef MIXERCORE_CCONFIG_H
#define MIXERCORE_CCONFIG_H




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
public: CConfig();
    static int GetConfig(const char *name, std::string &str);
    static int getGlobalValues();
    static int MAX_FACES=7;
    static int WIDTH=1280;
    static int HEIGHT=720;
    static std::string RTMP_MAIN="rtmp://wowza02.onevent.online/live/";
    static int FRAMERATE=30;
    static int MIXER_BITRATE=1024*1024*1.5;
    static int MIXER_GOP=30;
};


#endif //MIXERCORE_CCONFIG_H
