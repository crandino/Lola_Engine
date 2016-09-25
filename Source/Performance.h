#ifndef __PERFORMANCE_H__
#define __PERFORMANCE_H__

#include "Timer.h"
#include "PerfTimer.h"

#define HISTOGRAM_VALUES 60

class Performance
{
private:

	// Arrays for ImGui graphic performance info
	float frame_rate_hist[HISTOGRAM_VALUES];
	float miliseconds_hist[HISTOGRAM_VALUES];

	// Timers and DT
	PerfTimer	dt_timer;
	Timer		total_timer;
	float		ms_dt;
	float		sec_dt;

	int			frame_rate;					// Current framerate
	uint		avg_fps;
	float		seconds_since_startup;
	uint32		frames_on_last_update;
	uint64		frame_count;
	float		last_time;
	uint32		last_frames;

public:

	Performance()
	{
		total_timer.Start();
		dt_timer.Start();

		memset(frame_rate_hist, 0, HISTOGRAM_VALUES);
		memset(miliseconds_hist, 0, HISTOGRAM_VALUES);

		frame_rate = 0;
		avg_fps = 0;
		seconds_since_startup = 0.0f;
		frames_on_last_update = 0;
		frame_count = 0;

		last_time = 0.0f;
		last_frames = 0;
	}

	void PreUpdate()
	{
		// Calculate dt. Differential time since last frame.
		ms_dt = dt_timer.ReadMs();
		sec_dt = ms_dt / 1000.0f;

		// Starting timers
		dt_timer.Start();
	}

	void PostUpdate()
	{
		float curr_dt = dt_timer.ReadMs();

		// Delay to achieve cap framerate
		if (frame_rate != 0)
		{
			if ((1000.0f / frame_rate) - (curr_dt) > 0)
			{
				Uint32 delay = (Uint32)((1000 / frame_rate) - (curr_dt));
				SDL_Delay(delay);
				curr_dt = dt_timer.ReadMs(); // Or maybe, add "delay" althought is less precise.
			}
		}

		// Amount of frames since startup
		frame_count++;
		// Amount of time since game start (use a low resolution timer)
		seconds_since_startup = total_timer.ReadSec();
		// Average FPS for the whole game life
		avg_fps = frame_count / seconds_since_startup;

		// Amount of frames during the last second
		if (seconds_since_startup - last_time > 0.25f)
		{
			frames_on_last_update = (frame_count - last_frames) / (seconds_since_startup - last_time);
			last_time = seconds_since_startup;
			last_frames = frame_count;
		}

		for (int i = 0; i < HISTOGRAM_VALUES - 1; ++i)
		{
			frame_rate_hist[i] = frame_rate_hist[i + 1];
			miliseconds_hist[i] = miliseconds_hist[i + 1];
		}

		frame_rate_hist[HISTOGRAM_VALUES - 1] = frames_on_last_update;
		miliseconds_hist[HISTOGRAM_VALUES - 1] = curr_dt;
	}

	// Getters
	float &getMsDt() { return ms_dt; }
	float &getSecDt() { return sec_dt; }

	int &getFrameRate() { return frame_rate; }
	float *getFrameRateHist() {return frame_rate_hist; }
	float *getMilisecondsHist()	{ return miliseconds_hist; }
};

#endif //!__PERFORMANCE_H__

