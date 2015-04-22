/*
 * DISTRHO SoulForce, a DPF'ied SoulForce.
 * Copyright (c) 2006 Niall Moody
 * Copyright (C) 2015 Filipe Coelho <falktx@falktx.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef DISTRHO_UI_MVERB_HPP_INCLUDED
#define DISTRHO_UI_MVERB_HPP_INCLUDED

#include "DistrhoUI.hpp"

#include "ImageKnob.hpp"
#include "ImageSwitch.hpp"

#include "DistrhoArtworkSoulForce.hpp"

using DGL::Image;
using DGL::ImageKnob;
using DGL::ImageSwitch;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class DistrhoUISoulForce : public UI,
                           public ImageKnob::Callback,
                           public ImageSwitch::Callback
{
public:
    DistrhoUISoulForce();

protected:
    // -------------------------------------------------------------------
    // DSP Callbacks

    void d_parameterChanged(uint32_t index, float value) override;
    void d_programChanged(uint32_t index) override;

    // -------------------------------------------------------------------
    // Widget Callbacks

    void imageKnobDragStarted(ImageKnob* knob) override;
    void imageKnobDragFinished(ImageKnob* knob) override;
    void imageKnobValueChanged(ImageKnob* knob, float value) override;

    void imageSwitchClicked(ImageSwitch* imageButton, bool down) override;

    void onDisplay() override;

private:
    Image fImgBackground, fImgLedOff, fImgLedOn;
    ScopedPointer<ImageKnob> fKnobShape, fKnobFBack;
    ScopedPointer<ImageSwitch> fSwitchSource, fSwitchFoot;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistrhoUISoulForce)
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // DISTRHO_UI_MVERB_HPP_INCLUDED
