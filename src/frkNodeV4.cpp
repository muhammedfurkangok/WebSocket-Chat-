#include "frkNodeV4.h"

namespace WebSocket {
	void frkNodeV4::setSocketInfo() {
		this->m_adressLen = sizeof(this->m_socketInfo);
		std::memset(&this->m_socketInfo, 0, this->m_adressLen);
		this->m_socketInfo.sin_family = AF_INET;
		this->m_socketInfo.sin_port = htons(this->m_port);

	}
}