#include <benchmark/benchmark.h>

#include <algorithm>

#include <cassert>

#include "../draw2d/image.hpp"
#include "../draw2d/surface.hpp"

namespace
{
	void default_blit_earth_( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface( width, height );
		surface.clear();

		auto source = load_image( "assets/earth.png" );
		assert( source );

		for( auto _ : aState )
		{
			blit_masked( surface, *source, {0.f, 0.f} );

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically poossible.)
			benchmark::ClobberMemory(); 
		}

		// The following enables the benchmarking library to print information
		// about the memory bandwidth. The total number of bytes processed is
		// *approximatively* two times the total number of bytes in the blit,
		// accounding for both reading and writing. ("Approximatively" since
		// not all pixels are written.)
		auto const maxBlitX = std::min( width, source->get_width() );
		auto const maxBlitY = std::min( height, source->get_height() );

		aState.SetBytesProcessed( 2*maxBlitX*maxBlitY*4 * aState.iterations() );
	}

	// void my_other_blit_( benchmark::State& aState )
	// {
	// 		...
	// }
}

BENCHMARK( default_blit_earth_ )
	->Args( { 320, 240 } )
	->Args( { 7680, 4320 } )
;

//BENCHMARK( my_other_blit_ )
//	->Args( { 320, 240 } )
//	->Args( { 7680, 4320 } )
//;

BENCHMARK_MAIN();
