import json

from aiohttp import ClientSession

from .http_exception import HTTPException


class JSONDownloader:
    @staticmethod
    async def download(session: ClientSession, url: str, **kwargs) -> dict:
        async with session.get(url=url, **kwargs) as response:
            if not response.ok:
                response_text: str = await response.text()
                d: dict = json.loads(response_text)
                raise HTTPException(d.get('error'))

            return await response.json()
