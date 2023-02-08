#ifndef OCTAVE_MODELVIEWERSAMPLE_HPP
#define OCTAVE_MODELVIEWERSAMPLE_HPP

#include "Sample.hpp"

namespace octave::samples {

class ModelViewerSample : public Sample {
public:
	void OnLoad() override;
	void OnUpdate( const StepTimer& timer ) override;
	void OnRender() override;
	void OnUnload() override;
};

}  // namespace octave::samples

#endif  // OCTAVE_MODELVIEWERSAMPLE_HPP
