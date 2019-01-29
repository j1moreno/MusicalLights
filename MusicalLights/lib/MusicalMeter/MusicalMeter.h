#define AVERAGE_COUNT_BUFFER_SIZE 3

class MusicalMeter {

    public:
        MusicalMeter(int numberOfLevels, int startingPin);
        int GetNumberOfLevels();
        void CycleLevels(int delay_between_levels);
        void DisplayAdaptive(int audio_level);
        void Display(uint16_t audio_level);
        void SetZeroLevel(uint16_t zero_level);

    private:
        const int zero_thresh = 100;  // number of times sensor can read zero without reacting
        uint16_t zero_level_;
        int *led_pins_;
        int *thresholds_;
        int *led_states_;
        int number_of_levels_;
        int current_max_;
        int zero_count_;
        int thresh_count_;
        uint16_t average_buffer_[AVERAGE_COUNT_BUFFER_SIZE];
        uint8_t average_index_;
        void WriteToBuffer(uint16_t level);
        uint16_t GetBufferAverage();
        void RedefineThresholds(int audio_level);
        bool ExceedsZeroCount(int audio_level);
        void LedReact(int audio_level);
        void DisplayAudioLevelBasic(int audio_level, uint16_t max_value);
};