#include <Elementary.h>
#include <boost/bind.hpp>

#include <vector>

#include "window.h"
#include "actionslider.h"
#include "evasobject.h"
#include "elmtestharness.h"

using std::vector;

class DayselectorDayTest : public ElmTestHarness
{
public:

	DayselectorDayTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("DayselectorDayTest", "Dayselector Day Test")
		, control_(elm_dayselector_add(window_))
	{
		window_.setSize(500, 400);
		evas_object_size_hint_weight_set(control_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		elm_win_resize_object_add(window_, control_);

		days_.push_back(ELM_DAYSELECTOR_SUN);
		days_.push_back(ELM_DAYSELECTOR_MON);
		days_.push_back(ELM_DAYSELECTOR_TUE);
		days_.push_back(ELM_DAYSELECTOR_WED);
		days_.push_back(ELM_DAYSELECTOR_THU);
		days_.push_back(ELM_DAYSELECTOR_FRI);
		days_.push_back(ELM_DAYSELECTOR_SAT);

		return;
	}

	void setup()
	{
		window_.show();
		control_.show();

		vector<Elm_Dayselector_Day>::iterator it;
		for (it = days_.begin(); it < days_.end(); it++)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_dayselector_day_selected_set, boost::ref(control_), *it, EINA_TRUE),
					boost::bind(&DayselectorDayTest::checkDay, boost::ref(*this), *it, EINA_TRUE)
				)
			);

			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_dayselector_day_selected_set, boost::ref(control_), *it, EINA_FALSE),
					boost::bind(&DayselectorDayTest::checkDay, boost::ref(*this), *it, EINA_FALSE)
				)
			);

			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_dayselector_day_selected_set, boost::ref(control_), *it, EINA_TRUE),
					boost::bind(&DayselectorDayTest::checkDay, boost::ref(*this), *it, EINA_TRUE)
				)
			);
		}
	}

	void checkDay(Elm_Dayselector_Day day, Eina_Bool selected)
	{
		BOOST_CHECK_EQUAL(elm_dayselector_day_selected_get(control_, day), selected);
	}

private:
	Window				window_;
	EvasObject			control_;
	vector<Elm_Dayselector_Day>	days_;
};

class DayselectorLocaleTest : public ElmTestHarness
{
public:

	DayselectorLocaleTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("DayselectorLocaleTest", "Dayselector Locale Test")
		, control_(elm_dayselector_add(window_))
	{
		window_.setSize(500, 400);
		evas_object_size_hint_weight_set(control_, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		elm_win_resize_object_add(window_, control_);
		return;
	}

	void setup()
	{
		window_.show();
		control_.show();

		// toggle the first day of the week
		const Elm_Dayselector_Day weekstart = elm_dayselector_week_start_get(control_);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_dayselector_week_start_set, boost::ref(control_), ELM_DAYSELECTOR_WED),
				boost::bind(&DayselectorLocaleTest::checkStart, boost::ref(*this), ELM_DAYSELECTOR_WED)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_dayselector_week_start_set, boost::ref(control_), weekstart),
				boost::bind(&DayselectorLocaleTest::checkStart, boost::ref(*this), weekstart)
			)
		);

		// Toggle the length of the weekend
		const int len = elm_dayselector_weekend_length_get(control_);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_dayselector_weekend_length_set, boost::ref(control_), 7),
				boost::bind(&DayselectorLocaleTest::checkLength, boost::ref(*this), 7)
			)
		);

		queueCallback(
			ModifyCheckCallback(
				boost::bind(elm_dayselector_weekend_length_set, boost::ref(control_), len),
				boost::bind(&DayselectorLocaleTest::checkLength, boost::ref(*this), len)
			)
		);
	}

	void checkStart(Elm_Dayselector_Day weekstart)
	{
		BOOST_CHECK_EQUAL(elm_dayselector_week_start_get(control_), weekstart);
	}

	void checkLength(const int length)
	{
		BOOST_CHECK_EQUAL(elm_dayselector_weekend_length_get(control_), length);
	}

private:
	Window		window_;
	EvasObject	control_;
};

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(DaySelector)
	
		WAYLAND_ELM_HARNESS_TEST_CASE(DayselectorDayTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(DayselectorLocaleTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()