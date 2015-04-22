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

#include "DistrhoUISoulForce.hpp"
#include "DistrhoPluginSoulForce.hpp"

START_NAMESPACE_DISTRHO

namespace Art = DistrhoArtworkSoulForce;

// -----------------------------------------------------------------------

DistrhoUISoulForce::DistrhoUISoulForce()
    : UI(Art::backgroundWidth, Art::backgroundHeight),
      fImgBackground(Art::backgroundData, Art::backgroundWidth, Art::backgroundHeight, GL_BGR),
      fImgLedOff(Art::led_offData, Art::led_offWidth, Art::led_offHeight, GL_BGR),
      fImgLedOn(Art::led_onData, Art::led_onWidth, Art::led_onHeight, GL_BGR)
{
    // knobs
    fKnobShape = new ImageKnob(this,
                     Image(Art::knob_shapeData, Art::knob_shapeWidth, Art::knob_shapeHeight, GL_BGR));
    fKnobShape->setId(DistrhoPluginSoulForce::kParameterShape);
    fKnobShape->setAbsolutePos(17, 64);
    fKnobShape->setRange(0.0f, 1.0f);
    fKnobShape->setDefault(0.5f);
    fKnobShape->setImageLayerCount(92);
    fKnobShape->setCallback(this);

    fKnobFBack = new ImageKnob(this,
                     Image(Art::knob_feedbackData, Art::knob_feedbackWidth, Art::knob_feedbackHeight, GL_BGR));
    fKnobFBack->setId(DistrhoPluginSoulForce::kParameterFeedback);
    fKnobFBack->setAbsolutePos(176, 64);
    fKnobFBack->setRange(0.0f, 1.0f);
    fKnobFBack->setDefault(0.5f);
    fKnobFBack->setImageLayerCount(92);
    fKnobFBack->setCallback(this);

    // switches
    fSwitchSource = new ImageSwitch(this,
                        Image(Art::switch_leftData, Art::switch_leftWidth, Art::switch_leftHeight, GL_BGR),
                        Image(Art::switch_rightData, Art::switch_rightWidth, Art::switch_rightHeight, GL_BGR));
    fSwitchSource->setId(DistrhoPluginSoulForce::kParameterSource);
    fSwitchSource->setAbsolutePos(116, 191);
    fSwitchSource->setCallback(this);

    fSwitchFoot = new ImageSwitch(this,
                      Image(Art::button_upData, Art::button_upWidth, Art::button_upHeight, GL_BGR),
                      Image(Art::button_downData, Art::button_downWidth, Art::button_downHeight, GL_BGR));
    fSwitchFoot->setId(DistrhoPluginSoulForce::kParameterFootswitch);
    fSwitchFoot->setAbsolutePos(125, 282);
    fSwitchFoot->setCallback(this);

    // set initial values
    d_programChanged(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void DistrhoUISoulForce::d_parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case DistrhoPluginSoulForce::kParameterShape:
        fKnobShape->setValue(value);
        break;
    case DistrhoPluginSoulForce::kParameterFeedback:
        fKnobFBack->setValue(value);
        break;
    case DistrhoPluginSoulForce::kParameterSource:
        fSwitchSource->setDown(value > 0.5f);
        break;
    case DistrhoPluginSoulForce::kParameterFootswitch:
        fSwitchFoot->setDown(value > 0.5f);
        break;
    }
}

void DistrhoUISoulForce::d_programChanged(uint32_t index)
{
    switch(index)
    {
    case 0:
        fKnobShape->setValue(0.5f);
        fKnobFBack->setValue(0.0f);
        fSwitchSource->setDown(false);
        fSwitchFoot->setDown(true);
        break;
    case 1:
        fKnobShape->setValue(0.4f);
        fKnobFBack->setValue(0.0f);
        fSwitchSource->setDown(false);
        fSwitchFoot->setDown(true);
        break;
    case 2:
        fKnobShape->setValue(1.0f);
        fKnobFBack->setValue(0.0f);
        fSwitchSource->setDown(false);
        fSwitchFoot->setDown(true);
        break;
    case 3:
        fKnobShape->setValue(0.5f);
        fKnobFBack->setValue(1.0f);
        fSwitchSource->setDown(false);
        fSwitchFoot->setDown(true);
        break;
    case 4:
        fKnobShape->setValue(0.0f);
        fKnobFBack->setValue(1.0f);
        fSwitchSource->setDown(false);
        fSwitchFoot->setDown(true);
        break;
    case 5:
        fKnobShape->setValue(0.5f);
        fKnobFBack->setValue(1.0f);
        fSwitchSource->setDown(true);
        fSwitchFoot->setDown(true);
        break;
    case 6:
        fKnobShape->setValue(0.0f);
        fKnobFBack->setValue(1.0f);
        fSwitchSource->setDown(true);
        fSwitchFoot->setDown(true);
        break;
    case 7:
        fKnobShape->setValue(0.3f);
        fKnobFBack->setValue(0.5f);
        fSwitchSource->setDown(false);
        fSwitchFoot->setDown(true);
        break;
    case 8:
        fKnobShape->setValue(0.3f);
        fKnobFBack->setValue(0.5f);
        fSwitchSource->setDown(true);
        fSwitchFoot->setDown(true);
        break;
    }
}

// -----------------------------------------------------------------------
// Widget Callbacks

void DistrhoUISoulForce::imageKnobDragStarted(ImageKnob* knob)
{
    d_editParameter(knob->getId(), true);
}

void DistrhoUISoulForce::imageKnobDragFinished(ImageKnob* knob)
{
    d_editParameter(knob->getId(), false);
}

void DistrhoUISoulForce::imageKnobValueChanged(ImageKnob* knob, float value)
{
    d_setParameterValue(knob->getId(), value);
}

void DistrhoUISoulForce::imageSwitchClicked(ImageSwitch* button, bool down)
{
    const uint buttonId(button->getId());

    d_editParameter(buttonId, true);
    d_setParameterValue(buttonId, down ? 1.0f : 0.0f);
    d_editParameter(buttonId, false);
}

void DistrhoUISoulForce::onDisplay()
{
    fImgBackground.draw();

    if (fSwitchFoot->isDown())
        fImgLedOn.drawAt(123, 240);
    else
        fImgLedOff.drawAt(123, 240);
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new DistrhoUISoulForce();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
