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
        uint16_t zero_level_;
        int *led_pins_;
        int number_of_levels_;
        int current_max_;
        uint16_t average_buffer_[AVERAGE_COUNT_BUFFER_SIZE];
        uint8_t average_index_;
        void WriteToBuffer(uint16_t level);
        uint16_t GetBufferAverage();
        void DisplayAudioLevelBasic(int audio_level, uint16_t max_value);
};