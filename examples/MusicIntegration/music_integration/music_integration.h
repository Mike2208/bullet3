#ifndef MUSIC_INTEGRATION_H
#define MUSIC_INTEGRATION_H

#include <memory>
#include <music.hh>

class MusicIntegration
{
		static constexpr std::string_view FORERAM_JOINT_TORQUE_PORT = "forearm_joint_torque_port";
		static constexpr std::string_view FORERAM_JOINT_POS_PORT    = "forearm_joint_pos_port";

		class MusicSetup
		{
			public:
				MusicSetup(int argc, char **argv);
				~MusicSetup();

				MusicSetup(const MusicSetup&) = delete;
				MusicSetup &operator=(const MusicSetup&) = delete;

				MusicSetup(MusicSetup &&) = delete;
				MusicSetup &operator=(MusicSetup &&) = delete;

				constexpr MUSIC::Setup *get()
				{	return this->_setup;	}

				constexpr const MUSIC::Setup *get() const
				{	return this->_setup;	}

				static MusicIntegration CreateIntegration(int argc, char **argv, double step_size);
				MusicIntegration CreateIntegration(double step_size, MUSIC::ArrayData forarm_torque_buffer, MUSIC::ArrayData forarm_pos_buffer);

			private:
				MUSIC::Setup *_setup = nullptr;
		};

	public:
		~MusicIntegration() = default;

		MusicIntegration(const MusicIntegration &) = delete;
		MusicIntegration &operator=(const MusicIntegration &) = delete;

		MusicIntegration &operator=(MusicIntegration &&) = delete;

		static MusicIntegration &Reset(int argc, char **argv, double step_size);

	private:
		static std::unique_ptr<MusicIntegration> _ptr;

		MUSIC::ArrayData _forarm_torque_buffer;
		MUSIC::ArrayData _forarm_pos_buffer;

		MUSIC::ContInputPort  *_forearm_torque_port = nullptr;
		MUSIC::ContOutputPort *_forearm_pos_port = nullptr;

		MUSIC::Runtime _runtime;

		MusicIntegration(MusicIntegration &&) = default;
		MusicIntegration(MUSIC::ContInputPort *forearm_torque_port,
						 MUSIC::ContOutputPort *forearm_pos_port,
						 MUSIC::Runtime runtime);

		friend class MusicSetup;
};

#endif //MUSIC_INTEGRATION_H
