#ifndef IMP_TIMER_H_
#define IMP_TIMER_H_

class ImpTimer
{
public:
	ImpTimer();
	~ImpTimer();

	void start();
	void stop();
	void paused();
	void unpaused();

	int get_ticks();

	bool is_started();
	bool is_paused();

private:
	int start_tick_;
	int paused_tick_;

	bool is_paused_;
	bool is_started_;
};

#endif

//neu delay_time cang lon -> chuong trinh cang cham dan
//delay_time to khi FRAME_PER_SECOND cang nho
// vi fps cang nho =. time_one_frame cang to => time_one_frame - real_time cang to
//=> delay => cang to
//fps cang nho => chuong trinh cang cham