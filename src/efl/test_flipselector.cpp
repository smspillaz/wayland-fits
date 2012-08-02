#include <boost/filesystem.hpp>

#include <vector>
#include <string>

#include "templates.h"

using std::vector;
using std::string;

class Flipselector : public EvasObject
{
public:
	Flipselector(EvasObject &parent)
		: EvasObject::EvasObject(elm_flipselector_add(parent))
	{
		return;
	}
};

class FlipSelectorItemTest : public ElmTestHarness
{
public:
	FlipSelectorItemTest()
		: ElmTestHarness::ElmTestHarness()
		, window_("FlipselectorItemTest", "Flipselector Item Test")
		, control_(window_)
	{
		strings_.push_back("Nil");
		strings_.push_back("First");
		strings_.push_back("Second");
		strings_.push_back("Third");
		strings_.push_back("Fourth");
		strings_.push_back("Fifth");
		strings_.push_back("Sixth");
		strings_.push_back("Seventh");
		strings_.push_back("Eigth");
		strings_.push_back("Ninth");
		strings_.push_back("Tenth");

		return;
	}

	void setup()
	{
		Elm_Object_Item* pivot(
			elm_flipselector_item_append(
				control_, "PIVOT", NULL, NULL
			)
		);
		
		BOOST_CHECK_NE(pivot, static_cast<Elm_Object_Item*>(NULL));
		
		vector<Elm_Object_Item*> items;

		foreach (const std::string& label, strings_)
		{
			Elm_Object_Item* item = elm_flipselector_item_append(control_, label.c_str(), NULL, NULL);
			BOOST_CHECK_NE(item, static_cast<Elm_Object_Item*>(NULL));
			if (item != NULL)
			{
				items.push_back(item);
			}
		}

		window_.show();
		control_.show();

		control_.setSize(200, 200);
		control_.setPosition(50, 10);

		foreach_reverse (Elm_Object_Item* item, items)
		{
			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_flipselector_item_selected_set, item, EINA_TRUE),
					boost::bind(&FlipSelectorItemTest::checkItem, boost::ref(*this), item, pivot, EINA_TRUE)
				)
			);
			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_flipselector_item_selected_set, pivot, EINA_TRUE),
					boost::bind(&FlipSelectorItemTest::checkItem, boost::ref(*this), item, pivot, EINA_FALSE)
				)
			);
			queueCallback(
				ModifyCheckCallback(
					boost::bind(elm_flipselector_item_selected_set, item, EINA_TRUE),
					boost::bind(&FlipSelectorItemTest::checkItem, boost::ref(*this), item, pivot, EINA_TRUE)
				)
			);
		}

	}

	void checkItem(const Elm_Object_Item* item, const Elm_Object_Item* pivot, Eina_Bool expected)
	{
		BOOST_CHECK_EQUAL(elm_flipselector_item_selected_get(item), expected);
		BOOST_CHECK_EQUAL(elm_flipselector_item_selected_get(pivot), not expected);
	}

private:
	Window			window_;
	Flipselector	control_;
	vector<string>	strings_;
};

typedef ResizeObjectTest<Flipselector> FlipSelectorResizeTest;
typedef PositionObjectTest<Flipselector> FlipSelectorPositionTest;
typedef VisibleObjectTest<Flipselector> FlipSelectorVisibilityTest;

BOOST_AUTO_TEST_SUITE(EFL)

	BOOST_AUTO_TEST_SUITE(FlipSelector)

		WAYLAND_ELM_HARNESS_TEST_CASE(FlipSelectorItemTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FlipSelectorResizeTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FlipSelectorPositionTest)
		WAYLAND_ELM_HARNESS_TEST_CASE(FlipSelectorVisibilityTest)
	
	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
