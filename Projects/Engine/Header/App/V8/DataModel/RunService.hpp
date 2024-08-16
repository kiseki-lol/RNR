#pragma once
#include <App/V8/Tree/Instance.hpp>

namespace RNR
{
    class RunService : public Instance
    {
    private:
        float m_time;

        bool m_running;
        bool m_paused;

        virtual void addProperties(std::vector<ReflectionProperty>& properties);
    public:
        RunService();

        virtual std::string getClassName() { return "RunService"; }

        bool getRunning() { return m_running; }
        bool getPaused() { return m_paused; }

        void step(float delta);

        void run();
        void pause();
        void reset();
        void unpause();
    };
}