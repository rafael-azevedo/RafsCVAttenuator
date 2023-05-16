#include "plugin.hpp"

namespace rafulations {

struct RafsCVAttenuator : rack::Module {
    enum ParamIds {
        GAIN1_PARAM,
		GAIN2_PARAM,
		GAIN3_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        IN1_INPUT,
        IN2_INPUT,
        IN3_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
        OUT1_OUTPUT,
        OUT2_OUTPUT,
        OUT3_OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
		OUT1_POS_LIGHT, OUT1_NEG_LIGHT,
		OUT2_POS_LIGHT, OUT2_NEG_LIGHT,
		OUT3_POS_LIGHT, OUT3_NEG_LIGHT,
		NUM_LIGHTS
	};


    RafsCVAttenuator() {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        for (int c = 0; c < 3; c++) {
			configParam(GAIN1_PARAM + c, -1.f, 1.f, 0.f, string::f("Channel %d gain", c + 1));
			configInput(IN1_INPUT + c, string::f("Channel %d", c + 1));
			configOutput(OUT1_OUTPUT + c, string::f("Channel %d", c + 1));
		}
    }

    void process(const ProcessArgs& args) override {
        outputs[OUT1_OUTPUT].setVoltage(inputs[IN1_INPUT].getVoltage() * params[GAIN1_PARAM].getValue());
        outputs[OUT2_OUTPUT].setVoltage(inputs[IN2_INPUT].getVoltage() * params[GAIN2_PARAM].getValue());
        outputs[OUT3_OUTPUT].setVoltage(inputs[IN3_INPUT].getVoltage() * params[GAIN3_PARAM].getValue());
        lights[OUT1_POS_LIGHT + 2 ].setSmoothBrightness(fmaxf(0.0, 3 / 5.0), args.sampleTime);
        lights[OUT1_NEG_LIGHT + 2 ].setSmoothBrightness(fmaxf(0.0, -3 / 5.0), args.sampleTime);
    }
};

struct RafsCVAttenuatorWidget : rack::ModuleWidget {
    RafsCVAttenuatorWidget(RafsCVAttenuator* module) {
        setModule(module);
        setPanel(Svg::load(asset::plugin(pluginInstance, "res/RafsCVAttenuator.svg")));
        // (APP->window->loadSvg(asset::system("res/RafsCVAttenuator.svg")));
        addChild(createWidget<ScrewSilver>(Vec(15, 0)));
		addChild(createWidget<ScrewSilver>(Vec(15, 365)));
        // Add knobs
        addParam(createParam<Rogan1PSRed>(Vec(40, 40), module, RafsCVAttenuator::GAIN1_PARAM));
		addParam(createParam<Rogan1PSWhite>(Vec(40, 106), module, RafsCVAttenuator::GAIN2_PARAM));
		addParam(createParam<Rogan1PSGreen>(Vec(40, 172), module, RafsCVAttenuator::GAIN3_PARAM));


        // Add inputs
        addInput(createInput<PJ301MPort>(Vec(9, 245), module, RafsCVAttenuator::IN1_INPUT));
        addInput(createInput<PJ301MPort>(Vec(9, 281), module, RafsCVAttenuator::IN2_INPUT));
        addInput(createInput<PJ301MPort>(Vec(9, 317), module, RafsCVAttenuator::IN3_INPUT));

        // Add outputs
        addOutput(createOutput<PJ301MPort>(Vec(56, 245), module, RafsCVAttenuator::OUT1_OUTPUT));
        addOutput(createOutput<PJ301MPort>(Vec(56, 281), module, RafsCVAttenuator::OUT2_OUTPUT));
        addOutput(createOutput<PJ301MPort>(Vec(56, 317), module, RafsCVAttenuator::OUT3_OUTPUT));

        addChild(createLight<SmallLight<GreenRedLight>>(Vec(41, 254), module, RafsCVAttenuator::OUT1_POS_LIGHT));
		addChild(createLight<SmallLight<GreenRedLight>>(Vec(41, 290), module, RafsCVAttenuator::OUT2_POS_LIGHT));
		addChild(createLight<SmallLight<GreenRedLight>>(Vec(41, 326), module, RafsCVAttenuator::OUT3_POS_LIGHT));
   }
};

} // namespace rafulations

// Instantiate the plugin models
Model* modelRafsCVAttenuator = createModel<rafulations::RafsCVAttenuator, rafulations::RafsCVAttenuatorWidget>("RafsCVAttenuator");
