#include <catch2/catch_test_macros.hpp>

#include "LayerStack.hpp"

using namespace std;
using namespace Octave;

TEST_CASE( "initializes empty stack", "[LayerStack]" ) {
	LayerStack stack;
	REQUIRE( stack.IsEmpty() );
}

TEST_CASE( "pushes a layer onto the front of the stack", "[LayerStack]" ) {
	LayerStack stack;
	stack.PushLayer( make_unique<Layer>( "Test Layer" ) );

	REQUIRE( !stack.IsEmpty() );
	REQUIRE( ( *stack.begin() )->GetName() == "Test Layer" );
}

TEST_CASE( "pops the top-most layer from the stack", "[LayerStack]" ) {
	LayerStack stack;

	auto layer2 = make_unique<Layer>( "Layer 2" );

	stack.PushLayer( make_unique<Layer>( "Layer 1" ) );
	stack.PushLayer( move( layer2 ) );

	REQUIRE( stack.GetSize() == 2 );

	auto ptr = stack.PopLayer();

	REQUIRE( stack.GetSize() == 1 );
	REQUIRE( ptr != nullptr );
	REQUIRE( ptr->GetName() == "Layer 2" );
}
