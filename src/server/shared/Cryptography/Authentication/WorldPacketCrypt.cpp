#include "WorldPacketCrypt.h"
#include "Cryptography/HmacHash.h"
#include "Cryptography/BigNumber.h"

#include <cstring>

WorldPacketCrypt::WorldPacketCrypt() : PacketCrypt(SHA_DIGEST_LENGTH)
{
}

void WorldPacketCrypt::Init(BigNumber* K)
{
    uint8 ServerEncryptionKey[SEED_KEY_SIZE] = { 0x08, 0xF1, 0x95, 0x9F, 0x47, 0xE5, 0xD2, 0xDB, 0xA1, 0x3D, 0x77, 0x8F, 0x3F, 0x3E, 0xE7, 0x00 };
    HmacSha1 serverEncryptHmac(SEED_KEY_SIZE, (uint8*)ServerEncryptionKey);
    uint8 *encryptHash = serverEncryptHmac.ComputeHash(K);

    uint8 ServerDecryptionKey[SEED_KEY_SIZE] = { 0x40, 0xAA, 0xD3, 0x92, 0x26, 0x71, 0x43, 0x47, 0x3A, 0x31, 0x08, 0xA6, 0xE7, 0xDC, 0x98, 0x2A };
    HmacSha1 clientDecryptHmac(SEED_KEY_SIZE, (uint8*)ServerDecryptionKey);
    uint8 *decryptHash = clientDecryptHmac.ComputeHash(K);

    _clientDecrypt.Init(decryptHash);
    _serverEncrypt.Init(encryptHash);

    // Drop first 1024 bytes, as WoW uses ARC4-drop1024.
    uint8 syncBuf[1024];
    memset(syncBuf, 0, 1024);

    _serverEncrypt.UpdateData(1024, syncBuf);

    memset(syncBuf, 0, 1024);

    _clientDecrypt.UpdateData(1024, syncBuf);

    _initialized = true;
}