/*
 * Copyright © 2013 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make
 * no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "compositor.h"

namespace wfits {
namespace test {
namespace core {

Compositor::Compositor(const Display& display)
	: display_(display)
	, wl_compositor_(
		display.bind<wl_compositor>(
			"wl_compositor", &wl_compositor_interface))
{
	ASSERT(wl_compositor_ != NULL);

	wl_compositor_set_user_data(*this, this);
}

/*virtual*/ Compositor::~Compositor()
{
	wl_compositor_destroy(*this);
}

namespace wrapper {

	TEST(Compositor)
	{
		Display display;
		Compositor compositor(display);

		FAIL_UNLESS_EQUAL(&compositor.display(), &display);
		FAIL_IF((wl_compositor*)compositor == NULL);
		FAIL_UNLESS_EQUAL(wl_compositor_get_user_data(compositor), &compositor);
	}

} // namespace wrapper

} // namespace core
} // namespace test
} // namespace wfits
