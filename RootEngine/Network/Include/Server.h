#pragma once
#include <vector>

#define MAX_CLIENTS 12
namespace RootEngine
{
	namespace Network
	{
		typedef unsigned char ubyte;
		typedef signed char byte;

		struct Message
		{
			ubyte MessageID;
			byte RecipientID;
			bool Reliable;
			ubyte DataSize;
			byte* Data;
		};

		class Server abstract
		{
		public:
			Server(void);
			~Server(void);

			virtual bool Send(Message p_message) = 0;
			Message* PollMessage();
		protected:
			std::vector<Message> m_message;
		};
	}
}

