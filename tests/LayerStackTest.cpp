#include <catch2/catch_test_macros.hpp>

#include "LayerStack.hpp"

using namespace std;
using namespace Octave;

TEST_CASE( "initializes empty stack", "[LayerStack]" ) {
	LayerStack stack;
	REQUIRE( stack.IsEmpty() );
}

TEST_CASE( "move constructor", "[LayerStack]" ) {
	LayerStack stack1;
	stack1.PushLayer( make_unique<Layer>( "Layer 1" ) )
		.PushLayer( make_unique<Layer>( "Layer 2" ) );

	LayerStack stack2( std::move( stack1 ) );

	REQUIRE( stack1.GetSize() == 0 );
	REQUIRE( stack2.GetSize() == 2 );
	REQUIRE( ( *stack2.begin() )->GetName() == "Layer 2" );
	REQUIRE( ( *( stack2.begin() + 1 ) )->GetName() == "Layer 1" );
}

TEST_CASE( "move assignment operator", "[LayerStack]" ) {
	LayerStack stack1;
	stack1.PushLayer( make_unique<Layer>( "Layer 1" ) )
		.PushLayer( make_unique<Layer>( "Layer 2" ) );

	LayerStack stack2 = std::move( stack1 );

	REQUIRE( stack1.GetSize() == 0 );
	REQUIRE( stack2.GetSize() == 2 );
	REQUIRE( ( *stack2.begin() )->GetName() == "Layer 2" );
	REQUIRE( ( *( stack2.begin() + 1 ) )->GetName() == "Layer 1" );
}

TEST_CASE( "pushes a layer onto the front of the stack", "[LayerStack]" ) {
	LayerStack stack;
	stack.PushLayer( make_unique<Layer>( "Bottom Layer" ) )
		.PushLayer( make_unique<Layer>( "Top Layer" ) );

	REQUIRE( !stack.IsEmpty() );
	REQUIRE( stack.GetSize() == 2 );
	REQUIRE( ( *stack.begin() )->GetName() == "Top Layer" );
	REQUIRE( ( *stack.end() )->GetName() == "Bottom Layer" );
}

TEST_CASE( "pops the top-most layer from the stack", "[LayerStack]" ) {
	LayerStack stack;

	auto layer2 = make_unique<Layer>( "Layer 2" );

	stack.PushLayer( make_unique<Layer>( "Layer 1" ) );
	stack.PushLayer( std::move( layer2 ) );

	REQUIRE( stack.GetSize() == 2 );

	auto ptr = stack.PopLayer();

	REQUIRE( stack.GetSize() == 1 );
	REQUIRE( ptr != nullptr );
	REQUIRE( ptr->GetName() == "Layer 2" );
}
