#ifndef B7F1B5A8_4F75_4ECB_9219_F9C134690CCC
#define B7F1B5A8_4F75_4ECB_9219_F9C134690CCC

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "persistence.h"

#include <vector>
using namespace std;

namespace Detector{

class DetectorFSM;

class InputState
{
public:
	virtual void enter(DetectorFSM* fsm) = 0;
	virtual void toggle(DetectorFSM* fsm) = 0;
	virtual void exit(DetectorFSM* fsm) = 0;
	virtual ~InputState() {}
};



class Idle : public InputState
{
public:
	void enter(DetectorFSM* fsm);
	void toggle(DetectorFSM* fsm);
	void exit(DetectorFSM* fsm);
	static Idle& getInstance();

private:
	Idle() {}
	Idle(const Idle& other);
	Idle& operator=(const Idle& other);
};


class MouseClicked : public InputState
{
public:
	void enter(DetectorFSM* fsm);
	void toggle(DetectorFSM* fsm);
	void exit(DetectorFSM* fsm);
	static MouseClicked& getInstance();

private:
	MouseClicked() {}
	MouseClicked(const MouseClicked& other);
	MouseClicked& operator=(const MouseClicked& other);
	int m_lastEvent;
};


class DrawPreview : public InputState
{
public:
	void enter(DetectorFSM* fsm);
	void toggle(DetectorFSM* fsm);
	void exit(DetectorFSM* fsm);
	static DrawPreview& getInstance();

private:
	DrawPreview() {}
	DrawPreview(const DrawPreview& other);
	DrawPreview& operator=(const DrawPreview& other);
};









class DetectorFSM
{
public:
	DetectorFSM();
	inline InputState* getCurrentState() const { return currentState; }
	
	void process(int event, int x, int y, int kbdr);
	
	// This is where the magic happens
	void setState(InputState& newState);

	static DetectorFSM& getInstance();

	void Draw(cv::Mat& frame);

	void Persist(vector<Line>& list);

	int m_event, m_x, m_y, m_kbdr;

	vector<cv::Point> m_points;
	bool persist;

private:
	InputState* currentState;
};

};

#endif /* B7F1B5A8_4F75_4ECB_9219_F9C134690CCC */
