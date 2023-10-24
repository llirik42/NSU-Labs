from abc import ABC, abstractmethod


class SocketLike(ABC):
    @abstractmethod
    def fileno(self) -> int:
        pass
