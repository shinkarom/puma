#include <print>
#include <string>

#include "common.hpp"
#include "render.hpp"
#include "args.hxx"
#include "core.hpp"

int main(int argc, char *argv[])
{
	args::ArgumentParser parser("Puma");
	args::Positional<std::string> filename(parser, "filename", "The file to run Puma with");
	
	try {
		parser.ParseCLI(argc, argv);
	} catch (args::Help&) {
		std::cout<<parser;
	} catch (args::ParseError& e) {
		std::cerr<<e.what()<<std::endl;
		std::cerr<<parser;
		return 1;
	}
	
	core::init();
	render::init();
	
	if(filename) {
		auto st = args::get(filename);
		render::tryLoadFile(st);
	} 	
	
	bool shouldContinue;
	do {
		int16_t* audioBuffer = nullptr;
		uint32_t* videoBuffer = nullptr;
		core::frame(&videoBuffer, &audioBuffer);
		shouldContinue = render::frame(videoBuffer, audioBuffer);
	} while(shouldContinue);
	
	render::deinit();
	core::deinit();
	return 0;
}