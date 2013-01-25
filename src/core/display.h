#ifndef __WFITS_CORE_DISPLAY_H__
#define __WFITS_CORE_DISPLAY_H__

#include <map>
#include <wayland-client.h>
#include "common/test.h"

class Display
{
	typedef std::pair<uint32_t, uint32_t> GlobalInfo;
	typedef std::map<std::string, GlobalInfo> Globals;
public:
	Display();

	virtual ~Display();

	template <typename T>
	T* bind(
		const std::string& interface,
		const struct wl_interface *wl_interface) const
	{
		const Globals::const_iterator match(globals_.find(interface));

		ASSERT(match != globals_.end());

		return static_cast<T*>(
			wl_registry_bind(
				wl_registry_, match->second.first, wl_interface,
				match->second.second));
	}

	void roundtrip() const;
	void dispatch() const;

	operator wl_display*() const { return wl_display_; }

private:
	static void global(
		void*, struct wl_registry*, uint32_t, const char*, uint32_t);

	wl_display	*wl_display_;
	wl_registry	*wl_registry_;
	Globals		globals_;
};

#endif