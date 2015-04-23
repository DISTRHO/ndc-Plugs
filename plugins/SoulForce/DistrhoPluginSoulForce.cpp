/*
 * DISTRHO SoulForce, a DPF'ied SoulForce.
 * Copyright (C) 2006 Niall Moody
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

#include "DistrhoPluginSoulForce.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

DistrhoPluginSoulForce::DistrhoPluginSoulForce()
    : Plugin(kParameterCount, 9, 0), // 9 programs, 0 states
      coeff(0.5f),
      wave(0.0f),
      env(0.0f),
      footEnv(0.0f)
{
    // set initial values
    d_setProgram(0);
}

// -----------------------------------------------------------------------
// Init

void DistrhoPluginSoulForce::d_initParameter(uint32_t index, Parameter& parameter)
{
    parameter.hints      = kParameterIsAutomable;
    parameter.ranges.min = 0.0f;
    parameter.ranges.max = 1.0f;

    switch (index)
    {
    case kParameterShape:
        parameter.name       = "Shape";
        parameter.symbol     = "shape";
        parameter.ranges.def = 0.5f;
        break;
    case kParameterFeedback:
        parameter.name       = "FBack";
        parameter.symbol     = "fback";
        parameter.ranges.def = 0.0f;
        break;
    case kParameterSource:
        parameter.hints     |= kParameterIsBoolean;
        parameter.name       = "Source";
        parameter.symbol     = "source";
        parameter.ranges.def = 0.0f;
        break;
    case kParameterFootswitch:
        parameter.hints     |= kParameterIsBoolean;
        parameter.name       = "Foot";
        parameter.symbol     = "foot";
        parameter.ranges.def = 1.0f;
        break;
    }
}

void DistrhoPluginSoulForce::d_initProgramName(uint32_t index, d_string& programName)
{
    switch(index)
    {
    case 0:
        programName = "Default";
        break;
    case 1:
        programName = "Stay Down";
        break;
    case 2:
        programName = "Looking for the World";
        break;
    case 3:
        programName = "Guerilla Love";
        break;
    case 4:
        programName = "Tumble to the Power";
        break;
    case 5:
        programName = "Do Yourself a Favour";
        break;
    case 6:
        programName = "Past is Past";
        break;
    case 7:
        programName = "You and Only You";
        break;
    case 8:
        programName = "Soul Force";
        break;
    }
}

// -----------------------------------------------------------------------
// Internal data

float DistrhoPluginSoulForce::d_getParameterValue(uint32_t index) const
{
    return parameters[index];
}

void DistrhoPluginSoulForce::d_setParameterValue(uint32_t index, float value)
{
    parameters[index] = value;

    switch(index)
    {
    case kParameterShape:
        coeff = (1.0f-(1.0f/((value * 0.99f)+0.005f)));
        break;
    case kParameterFeedback:
        if (d_isZero(value))
            coeff = (1.0f-(1.0f/((parameters[kParameterShape] * 0.99f)+0.005f)));
        break;
    }
}

void DistrhoPluginSoulForce::d_setProgram(uint32_t index)
{
    switch(index)
    {
    case 0:
        parameters[kParameterShape]      = 0.5f;
        parameters[kParameterFeedback]   = 0.0f;
        parameters[kParameterSource]     = 0.0f;
        parameters[kParameterFootswitch] = 1.0f;
        break;
    case 1:
        parameters[kParameterShape]      = 0.4f;
        parameters[kParameterFeedback]   = 0.0f;
        parameters[kParameterSource]     = 0.0f;
        parameters[kParameterFootswitch] = 1.0f;
        break;
    case 2:
        parameters[kParameterShape]      = 1.0f;
        parameters[kParameterFeedback]   = 0.0f;
        parameters[kParameterSource]     = 0.0f;
        parameters[kParameterFootswitch] = 1.0f;
        break;
    case 3:
        parameters[kParameterShape]      = 0.5f;
        parameters[kParameterFeedback]   = 1.0f;
        parameters[kParameterSource]     = 0.0f;
        parameters[kParameterFootswitch] = 1.0f;
        break;
    case 4:
        parameters[kParameterShape]      = 0.0f;
        parameters[kParameterFeedback]   = 1.0f;
        parameters[kParameterSource]     = 0.0f;
        parameters[kParameterFootswitch] = 1.0f;
        break;
    case 5:
        parameters[kParameterShape]      = 0.5f;
        parameters[kParameterFeedback]   = 1.0f;
        parameters[kParameterSource]     = 1.0f;
        parameters[kParameterFootswitch] = 1.0f;
        break;
    case 6:
        parameters[kParameterShape]      = 0.0f;
        parameters[kParameterFeedback]   = 1.0f;
        parameters[kParameterSource]     = 1.0f;
        parameters[kParameterFootswitch] = 1.0f;
        break;
    case 7:
        parameters[kParameterShape]      = 0.3f;
        parameters[kParameterFeedback]   = 0.5f;
        parameters[kParameterSource]     = 0.0f;
        parameters[kParameterFootswitch] = 1.0f;
        break;
    case 8:
        parameters[kParameterShape]      = 0.3f;
        parameters[kParameterFeedback]   = 0.5f;
        parameters[kParameterSource]     = 1.0f;
        parameters[kParameterFootswitch] = 1.0f;
        break;
    }
}

// -----------------------------------------------------------------------
// Process

void DistrhoPluginSoulForce::d_run(const float** inputs, float** outputs, uint32_t frames)
{
    float tempf, tempf2;
    float inLeft, inRight;

    //frames = sampleFrames;

    // Calculate audio.
    for (uint32_t i=0; i<frames; ++i)
    {
        // For footEnv, later.
        inLeft  = inputs[0][i];
        inRight = inputs[1][i];

        // Update coeff, if necessary.
        if (parameters[kParameterFeedback] > 0.0f)
        {
            if (parameters[kParameterSource] > 0.5f)
            {
                tempf2  = (1.0f-parameters[kParameterFeedback]) * parameters[kParameterShape];
                tempf2 += parameters[kParameterFeedback] * wave;
                coeff   = (1.0f-(1.0f/((tempf2 * 0.99f)+0.005f)));
            }
            else
            {
                if ((tempf = std::abs(inLeft)) > env)
                {
                    env = tempf;
                }
                else if (env > 0.0f)
                {
                    env -= 0.001f;
                    if (env < 0.0f)
                        env = 0.0f;
                }

                tempf2  = (1.0f-parameters[kParameterFeedback]) * parameters[kParameterShape];
                tempf2 += parameters[kParameterFeedback] * env;
                coeff   = (1.0f-(1.0f/((tempf2 * 0.99f)+0.005f)));
            }
        }

        tempf = inLeft;
        if (tempf > 0.0f)
        {
            tempf = tempf/(tempf+(coeff*(tempf-1.0f)));
        }
        else if (d_isZero(tempf))
        {
            tempf = 0.0f;
        }
        else
        {
            tempf  = std::abs(tempf);
            tempf  = 1.0f - (tempf/(tempf+(coeff*(tempf-1.0f))));
            tempf -= 1.0f;
        }
        if (parameters[kParameterShape] < 0.5f)
            tempf *= ((0.5f-parameters[kParameterShape])*16.0f)+1.0f;
        if (parameters[kParameterSource] > 0.5f)
            tempf *= 1.0f + (parameters[kParameterFeedback] * 2.0f);
        outputs[0][i] = tempf;

        tempf = inRight;
        if (tempf > 0.0f)
        {
            tempf = tempf/(tempf+(coeff*(tempf-1.0f)));
        }
        else if (d_isZero(tempf))
        {
            tempf = 0.0f;
        }
        else
        {
            tempf  = std::abs(tempf);
            tempf  = 1.0f - (tempf/(tempf+(coeff*(tempf-1.0f))));
            tempf -= 1.0f;
        }
        if (parameters[kParameterShape] < 0.5f)
            tempf *= ((0.5f-parameters[kParameterShape])*16.0f)+1.0f;
        if (parameters[kParameterSource] > 0.5f)
            tempf *= 1.0f + (parameters[kParameterFeedback] * 2.0f);
        outputs[1][i] = tempf;

        outputs[0][i] = std::tanh(outputs[0][i]) * 0.9f;
        outputs[1][i] = std::tanh(outputs[1][i]) * 0.9f;
        wave = outputs[0][i];

        // Handle footswitch.
        outputs[0][i] *= footEnv;
        outputs[0][i] += (1.0f-footEnv) * inLeft;
        outputs[1][i] *= footEnv;
        outputs[1][i] += (1.0f-footEnv) * inRight;

        if (parameters[kParameterFootswitch] > 0.5f && footEnv < 1.0f)
            footEnv += 0.01f;
        else if (parameters[kParameterFootswitch] < 0.5f && footEnv > 0.0f)
            footEnv -= 0.01f;
    }
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new DistrhoPluginSoulForce();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
