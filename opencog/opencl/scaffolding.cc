/**
 * OpenCL scaffolding.
 *
 * This provides minimalistic scaffolding to allow OpenCL experiments
 * to take place.
 *
 * Copyright (c) 2025 Linas Vepstas
 */

#define CL_HPP_MINIMUM_OPENCL_VERSION 200
#define CL_HPP_TARGET_OPENCL_VERSION 300
#define CL_HPP_ENABLE_EXCEPTIONS

#include <CL/opencl.hpp>
#include <iostream>
#include <fstream>

void use_dev(cl::Device ocldev, const char* srcfile)
{
#if 0
	auto dimensions = ocldev.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
	size_t global_dim = dimensions[0] * dimensions[1] * dimensions[2];
#endif

	// Copy in source code. Must be a better way!?
	printf("Reading sourcefile %s\n", srcfile);
	std::ifstream srcfm(srcfile);
	// std::stringstream buffer;
	// buffer << srcfm.rdbuf();
	std::string src(std::istreambuf_iterator<char>(srcfm),
		(std::istreambuf_iterator<char>()));

	cl::Program::Sources sources;
	sources.push_back(src);

	cl::Context context(ocldev);
	cl::Program program(context, sources);

	// Compile
	printf("Compiling sourcefile %s\n", srcfile);
	try
	{
		// Specifying flags causes exception.
		// program.build("-cl-std=CL1.2");
		program.build("");
	}
	catch (const cl::Error& e)
	{
		printf("Compile failed! %s\n", e.what());
		printf("Log >>%s<<\n",
			program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(ocldev).c_str());
		exit(1);
	}

	// Set up I/O
	char buf[256];
	// CL_MEM_USE_HOST_PTR
	cl::Buffer memBuf(context,
		CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY,
		sizeof(buf));
	int err;
	cl::Kernel kernel(program, "HelloWorld", &err);
	kernel.setArg(0, memBuf);

	// Launch
	cl::CommandQueue queue(context, ocldev);

	cl::Event *event_handler = new cl::Event();
	queue.enqueueNDRangeKernel(kernel,
		cl::NullRange,
		// cl::NDRange(global_dim, 1, 1),
		cl::NDRange(16, 1, 1),
		cl::NullRange,
		nullptr, event_handler);

	// this is deprecated API.
	// queue.enqueueTask(kernel);

	event_handler->wait();
	printf("Done waiting on exec\n");

	// queue.enqueueReadBuffer(memBuf, CL_TRUE, 0, sizeof(buf), buf);
	queue.enqueueReadBuffer(memBuf, CL_TRUE, 0, sizeof(buf), buf,
		nullptr, event_handler);
	event_handler->wait();
	printf("Done waiting on read\n");

	delete event_handler;

	printf("pong >>%s<<\n", buf);
}

/// Print rudimentary report of available OpenCL hardware.
void report_hardware(void)
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	printf("OpenCL Hardware report\n");
	printf("Should match what the `clinfo` tool reports.\n");
	printf("Found %ld plaforms:\n", platforms.size());

	for (const auto& plat : platforms)
	{
		std::string pname = plat.getInfo<CL_PLATFORM_NAME>();
		std::string pvend = plat.getInfo<CL_PLATFORM_VENDOR>();
		std::string pvers = plat.getInfo<CL_PLATFORM_VERSION>();
		printf("Platform: %s\n", pname.c_str());
		printf("\tVendor: %s\n", pvend.c_str());
		printf("\tVersion: %s\n", pvers.c_str());

		std::vector<cl::Device> devices;
		// plat.getDevices(CL_DEVICE_TYPE_ALL, &devices);
		plat.getDevices(CL_DEVICE_TYPE_GPU, &devices);

		printf("\tThis platform has %ld GPU devices:\n", devices.size());

		for (const auto& dev: devices)
		{
			std::string dname = dev.getInfo<CL_DEVICE_NAME>();
			std::string dvers = dev.getInfo<CL_DEVICE_VERSION>();
			printf("\t\tDevice %s\n", dname.c_str());
			printf("\t\tVersion %s\n", dvers.c_str());
			auto dimensions = dev.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
			printf("\t\tMax dimensions: %ld x %ld x %ld\n",
				dimensions[0], dimensions[1], dimensions[2]);
			printf("\n");
		}
		printf("\n");
	}
	printf("\n");
}

/// Return the first device that has platsubstr and devsubstr as
/// subtrings in the platform and device name.
cl::Device find_device(const char* platsubstr, const char* devsubstr)
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	for (const auto& plat : platforms)
	{
		std::string pname = plat.getInfo<CL_PLATFORM_NAME>();
		if (pname.find(platsubstr) == std::string::npos)
			continue;

		std::vector<cl::Device> devices;
		// plat.getDevices(CL_DEVICE_TYPE_ALL, &devices);
		plat.getDevices(CL_DEVICE_TYPE_GPU, &devices);

		for (const cl::Device& dev: devices)
		{
			std::string dname = dev.getInfo<CL_DEVICE_NAME>();
			if (dname.find(devsubstr) == std::string::npos)
				continue;

			// Return first matching device.
			return dev;
		}
	}

	static const cl::Device nulldev;
	return nulldev;
}

int main(int argc, char* argv[])
{
	report_hardware();

	cl::Device ocldev = find_device("", "AMD");
	std::string dname = ocldev.getInfo<CL_DEVICE_NAME>();
	printf("Will use: %s\n", dname.c_str());

	use_dev(ocldev, "hello.cl");
}
