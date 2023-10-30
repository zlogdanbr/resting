#include "unit_testing.h"

namespace testing
{

	template<typename function, typename T, typename M>
	struct F1
	{
		// this function is called to validate the output of the input IN
		// against the expected output expected value
		// you are supposed to provide the == operator if the data type
		// does not support it already
		bool
			myFunctionTester(function& f,
				const M& in,
				T& expected_value)  noexcept
		{

			auto calculated_val = f(in);

			std::cout << "expected value: " << expected_value << std::endl;
			std::cout << "calculated value: " << calculated_val << std::endl;

			return expected_value == calculated_val;

		}
	};



	// the input data is passed as an vector so you need to
	// add your test case input data to a vector and
	// provide  the vector of expected values
	// f is a template for the function that you will test
	// so it requires a parameter of input
	template<typename IN, typename OUT, typename function>
	struct F2
	{
		void
			UnitTesting(const std::vector<IN>& InData,
				const std::vector<OUT>& Expected,
				function& f) noexcept
		{

			int ok = 0;
			int failed = 0;
			int cnt = 0;
			for (const auto& d : InData)
			{
				std::cout << "\nRunning test case " << cnt << "\n";
				if (myFunctionTester(f, d, Expected[cnt]) == true)
				{
					std::cout << "Test case " << cnt << " was suscessfull" << std::endl;
					ok++;
				}
				else
				{
					std::cout << "Test case " << cnt << " failed" << std::endl;
					failed++;
				}
				cnt++;
			}

			std::cout << "_____________________________________________" << "\n";
			std::cout << "Failed: " << failed << std::endl;
			std::cout << "Sucessfull: " << ok << std::endl;
			std::cout << "_____________________________________________" << "\n";

		};

	};
};

