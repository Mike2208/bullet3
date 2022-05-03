#include "music_integration/music_integration.h"


std::unique_ptr<MusicIntegration> MusicIntegration::_ptr = nullptr;

MusicIntegration::MusicSetup::MusicSetup(int argc, char **argv)
    : _setup(new MUSIC::Setup(argc, argv))
{}

MusicIntegration::MusicSetup::~MusicSetup()
{
	if(this->_setup)
	{
		delete this->_setup;
		this->_setup = nullptr;
	}
}

MusicIntegration MusicIntegration::MusicSetup::CreateIntegration(int argc, char **argv, double step_size)
{
	MusicSetup setup(argc, argv);
	return setup.CreateIntegration(step_size);
}

MusicIntegration MusicIntegration::MusicSetup::CreateIntegration(double step_size, MUSIC::ArrayData forarm_torque_buffer, MUSIC::ArrayData forarm_pos_buffer)
{
	MUSIC::Setup *setup = this->_setup;

	// Port connections
	MUSIC::ContInputPort *forearm_torque_port = setup->publishContInput(FORERAM_JOINT_TORQUE_PORT.data());
	MUSIC::ContOutputPort *forearm_pos_port = setup->publishContOutput(FORERAM_JOINT_POS_PORT.data());

	// Create Runtime (also destroys setup)
	this->_setup = nullptr;
	return MusicIntegration(forearm_torque_port, forearm_pos_port, MUSIC::Runtime(setup, step_size));
}

MusicIntegration &MusicIntegration::Reset(int argc, char **argv, double step_size)
{
	_ptr.reset(new MusicIntegration(MusicSetup::CreateIntegration(argc, argv, step_size)));

	return *_ptr.get();
}

MusicIntegration::MusicIntegration(MUSIC::ContInputPort *forearm_torque_port, MUSIC::ContOutputPort *forearm_pos_port, MUSIC::Runtime runtime)
    : _forarm_torque_buffer(new double[1], MPI::DOUBLE,	)
      _forearm_torque_port(forearm_torque_port),
      _forearm_pos_port(forearm_pos_port),
      _runtime(runtime)
{}
