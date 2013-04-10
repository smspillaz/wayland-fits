//---- The following set of defines/includes make this work for 1.7.x ----//
#define ELM_INTERNAL_API_ARGESFSDFEFC
#include <Evas.h>
#include <Elementary.h>
#include <elm_widget.h>
//-----------------------------------------------------------------------//

#include <elm_widget_actionslider.h>

#include <vector>
#include <linux/input.h>

#include "templates.h"

using std::vector;

class Actionslider : public EvasObject
{
public:
	Actionslider(EvasObject &parent)
		: EvasObject::EvasObject(elm_actionslider_add(parent))
	{
		return;
	}

	Geometry getButtonGeometry() const
	{
#pragma GCC diagnostic ignored "-fpermissive"
		ELM_ACTIONSLIDER_DATA_GET(*this, sd);
#pragma GCC diagnostic error "-fpermissive"

		EvasObject o(sd->drag_button_base, false);
		return o.getGeometry();
	}
};

class ActionsliderIndicatorTest : public ElmTestHarness
{
public:
	ActionsliderIndicatorTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ActionsliderIndicatorTest", "Actionslider Position Test")
		, control_(window_)
	{
		positions_.push_back(ELM_ACTIONSLIDER_LEFT);
		positions_.push_back(ELM_ACTIONSLIDER_CENTER);
		positions_.push_back(ELM_ACTIONSLIDER_LEFT);
		positions_.push_back(ELM_ACTIONSLIDER_RIGHT);
		positions_.push_back(ELM_ACTIONSLIDER_CENTER);
		positions_.push_back(ELM_ACTIONSLIDER_RIGHT);
		positions_.push_back(ELM_ACTIONSLIDER_LEFT);
		positions_.push_back(ELM_ACTIONSLIDER_RIGHT);
		positions_.push_back(ELM_ACTIONSLIDER_CENTER);
	}

	void setup()
	{
		window_.show();
		control_.show();

		control_.setSize(200, 100);
		control_.setPosition(50, 10);

		foreach (Elm_Actionslider_Pos p, positions_) {
			queueStep(boost::bind(elm_actionslider_indicator_pos_set, boost::ref(control_), p));
			queueStep(boost::bind(&ActionsliderIndicatorTest::checkPos, boost::ref(*this), p));
		}
	}

	void checkPos(const Elm_Actionslider_Pos expected)
	{
		FAIL_UNLESS_EQUAL(elm_actionslider_indicator_pos_get(control_), expected);
	}

private:
	Window				window_;
	Actionslider			control_;
	vector<Elm_Actionslider_Pos>	positions_;
};

class ActionSliderUserTest : public ElmTestHarness
{
public:
	ActionSliderUserTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("ActionSliderUserTest", "ActionSlider User Test")
		, slider_(window_)
		, position_()
		, selection_()
	{
		return;
	}

	void setup()
	{
		window_.show();

		int w(200), h(50);
		int x(window_.getWidth() / 2 - w / 2);
		int y(window_.getHeight() / 2 - h / 2);

		slider_.setSize(w, h);
		slider_.setPosition(x, y);

		elm_object_part_text_set(slider_, "left", "Left");
		elm_object_part_text_set(slider_, "center", "Center");
		elm_object_part_text_set(slider_, "right", "Right");

		evas_object_smart_callback_add(slider_, "selected", onSelect, this);
		evas_object_smart_callback_add(slider_, "pos_changed", onChange, this);

		slider_.show();

		boost::function<void (Elm_Actionslider_Pos)> fn =
			boost::bind(&ActionSliderUserTest::moveSliderTo, boost::ref(*this), _1);

		queueStep(boost::bind(fn, ELM_ACTIONSLIDER_CENTER));
		queueStep(boost::bind(fn, ELM_ACTIONSLIDER_RIGHT));
		queueStep(boost::bind(fn, ELM_ACTIONSLIDER_LEFT));
		queueStep(boost::bind(fn, ELM_ACTIONSLIDER_RIGHT));
		queueStep(boost::bind(fn, ELM_ACTIONSLIDER_CENTER));
		queueStep(boost::bind(fn, ELM_ACTIONSLIDER_LEFT));
	}

	void moveSliderTo(const Elm_Actionslider_Pos pos)
	{
		Application::yield(0.01*1e6);

		Geometry gw(getSurfaceGeometry(window_.get_wl_surface()));
		Geometry gf(window_.getFramespaceGeometry());
		Geometry gb(slider_.getButtonGeometry());
		Geometry gs(slider_.getGeometry());

		setGlobalPointerPosition(
			gw.x + gf.x + gb.x + gb.width / 2,
			gw.y + gf.y + gb.y + gb.height / 2
		);
		pointerKeyPress(BTN_LEFT, 1);

		std::string position, selection;
		int32_t x;

		switch (pos) {
			case ELM_ACTIONSLIDER_LEFT:
				position = "left";
				selection = "Left";
				x = gw.x + gf.x + gs.x + gb.width / 2;
				break;
			case ELM_ACTIONSLIDER_CENTER:
				position = "center";
				selection = "Center";
				x = gw.x + gf.x + gs.x + gs.width / 2;
				break;
			default:
				position = "right";
				selection = "Right";
				x = gw.x + gf.x + gs.x + gs.width - gb.width / 2;
				break;
		}

		setGlobalPointerPosition(x, gw.y + gf.y + gb.y + gb.height / 2);

		std::cout << "...checking for position changed event on '"
			<< position << "'" << std::endl;
		while (position_ != position) {
			Application::yield();
		}

		pointerKeyPress(BTN_LEFT, 0);

		std::cout << "...checking for selected event on '"
			<< selection << "'" << std::endl;
		while (selection_ != selection) {
			Application::yield();
		}
	}

	static void onSelect(void* data, Evas_Object*, void* info)
	{
		ActionSliderUserTest *test = static_cast<ActionSliderUserTest*>(data);
		test->selection_ = std::string(static_cast<char*>(info));
		std::cout << "...received selected event on '"
			<< test->selection_ << "'" << std::endl;
	}

	static void onChange(void* data, Evas_Object*, void* info)
	{
		ActionSliderUserTest *test = static_cast<ActionSliderUserTest*>(data);
		test->position_ = std::string(static_cast<char*>(info));;
		std::cout << "...received position changed event on '"
			<< test->position_ << "'" << std::endl;
	}

private:
	Window		window_;
	Actionslider	slider_;
	std::string	position_;
	std::string	selection_;
};

typedef ResizeObjectTest<Actionslider> ActionsliderResizeTest;
typedef PositionObjectTest<Actionslider> ActionsliderPositionTest;
typedef VisibleObjectTest<Actionslider> ActionsliderVisibilityTest;

WAYLAND_ELM_HARNESS_TEST_CASE(ActionsliderResizeTest, "ActionSlider")
WAYLAND_ELM_HARNESS_TEST_CASE(ActionsliderPositionTest, "ActionSlider")
WAYLAND_ELM_HARNESS_TEST_CASE(ActionsliderVisibilityTest, "ActionSlider")
WAYLAND_ELM_HARNESS_TEST_CASE(ActionsliderIndicatorTest, "ActionSlider")
WAYLAND_ELM_HARNESS_TEST_CASE(ActionSliderUserTest, "ActionSlider")

