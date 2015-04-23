/*
 * DISTRHO AmplitudeImposer, a DPF'ied AmplitudeImposer.
 * Copyright (C) 2004 Niall Moody
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

#include "DistrhoPluginAmplitudeImposer.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

struct twofloats {
    float left;
    float right;
};

// -----------------------------------------------------------------------

DistrhoPluginAmplitudeImposer::DistrhoPluginAmplitudeImposer()
    : Plugin(kParameterCount, 1, 0), // 1 program, 0 states
      fDepth(1.0f),
      fThreshold(0.5f),
      ampEnvelope_l(0.0f),
      ampEnvelope_r(0.0f),
      audioEnvelope_l(0.0f),
      audioEnvelope_r(0.0f),
      envDecay(0.0001f) {}

// -----------------------------------------------------------------------
// Init

void DistrhoPluginAmplitudeImposer::d_initParameter(uint32_t index, Parameter& parameter)
{
    parameter.hints      = kParameterIsAutomable;
    parameter.ranges.min = 0.0f;
    parameter.ranges.max = 1.0f;

    switch (index)
    {
    case kParameterDepth:
        parameter.name       = "Depth";
        parameter.symbol     = "depth";
        parameter.ranges.def = 1.0f;
        break;
    case kParameterThreshold:
        parameter.name       = "Thres";
        parameter.symbol     = "thres";
        parameter.ranges.def = 0.5f;
        break;
    }
}

void DistrhoPluginAmplitudeImposer::d_initProgramName(uint32_t index, d_string& programName)
{
    switch(index)
    {
    case 0:
        programName = "Default";
        break;
    }
}

// -----------------------------------------------------------------------
// Internal data

float DistrhoPluginAmplitudeImposer::d_getParameterValue(uint32_t index) const
{
    switch(index)
    {
    case kParameterDepth:
        return fDepth;
    case kParameterThreshold:
        return fThreshold;
    default:
        return 0.0f;
    }
}

void DistrhoPluginAmplitudeImposer::d_setParameterValue(uint32_t index, float value)
{
    switch(index)
    {
    case kParameterDepth:
        fDepth = value;
        break;
    case kParameterThreshold:
        fThreshold = value;
        break;
    }
}

void DistrhoPluginAmplitudeImposer::d_setProgram(uint32_t index)
{
    switch(index)
    {
    case 0:
        fDepth     = 1.0f;
        fThreshold = 0.5f;
        break;
    }
}

// -----------------------------------------------------------------------
// Process

void DistrhoPluginAmplitudeImposer::d_activate()
{
    ampEnvelope_l = 0.0f;
    ampEnvelope_r = 0.0f;
    audioEnvelope_l = 0.0f;
    audioEnvelope_r = 0.0f;
    envDecay = 0.0001f;
}

void DistrhoPluginAmplitudeImposer::d_run(const float** inputs, float** outputs, uint32_t frames)
{
    const float* const in1  =  inputs[0];
    const float* const in2  =  inputs[1];
    const float* const in3  =  inputs[2];
    const float* const in4  =  inputs[3];
    /* */ float* const out1 = outputs[0];
    /* */ float* const out2 = outputs[1];

    float tmp;
    twofloats tempf;
    twofloats tempin;

    for (uint32_t i=0; i<frames; ++i)
    {
        // calculate envelope from 1st two inputs
        tmp = std::abs(in1[i]);
        /**/ if (tmp > ampEnvelope_l)
            ampEnvelope_l = tmp;
        else if (tmp < ampEnvelope_l)
            ampEnvelope_l -= envDecay;

        tmp = std::abs(in2[i]);
        /**/ if (tmp > ampEnvelope_r)
            ampEnvelope_r = tmp;
        else if (tmp < ampEnvelope_r)
            ampEnvelope_r -= envDecay;

        // calculate envelope from 2nd two inputs
        tmp = std::abs(in3[i]);
        /**/ if (tmp > audioEnvelope_l)
            audioEnvelope_l = tmp;
        else if (tmp < audioEnvelope_l)
            audioEnvelope_l -= envDecay;

        tmp = std::abs(in4[i]);
        /**/ if (tmp > audioEnvelope_r)
            audioEnvelope_r = tmp;
        else if (tmp < audioEnvelope_r)
            audioEnvelope_r -= envDecay;

        // make sure we're not multiplying by a negative number
        if (ampEnvelope_l < 0.0f)
            ampEnvelope_l = 0.0f;
        if (ampEnvelope_r < 0.0f)
            ampEnvelope_r = 0.0f;
        if (audioEnvelope_l < 0.0f)
            audioEnvelope_l = 0.0f;
        if (audioEnvelope_r < 0.0f)
            audioEnvelope_r = 0.0f;

        // work out whether we need to multiply audio input
        if (audioEnvelope_l > fThreshold)
        {
            tempin.left = in3[i];
        }
        else
        {
            if (audioEnvelope_l > 0.001f)
                tempin.left = in3[i] * (fThreshold/audioEnvelope_l);
            else
                tempin.left = in3[i] * (fThreshold/0.001f); //so it'll decay away smoothly
        }

        if (audioEnvelope_r > fThreshold)
        {
            tempin.right = in4[i];
        }
        else
        {
            if (audioEnvelope_r > 0.001f)
                tempin.right = in4[i] * (fThreshold/audioEnvelope_r);
            else
                tempin.right = in4[i] * (fThreshold/0.001f);
        }

        // calculate output
        tempf.left   = tempin.left * ampEnvelope_l;
        tempf.left  *= fDepth;
        tempf.left   = tempf.left + ((1.0f-fDepth)*tempin.left);
        tempf.right  = tempin.right * ampEnvelope_r;
        tempf.right *= fDepth;
        tempf.right  = tempf.right + ((1.0f-fDepth)*tempin.right);

        out1[i] = tempf.left;
        out2[i] = tempf.right;
    }
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new DistrhoPluginAmplitudeImposer();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
