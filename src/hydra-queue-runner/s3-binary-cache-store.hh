#pragma once

#include "binary-cache-store.hh"

#include <atomic>

namespace Aws { namespace Client { class ClientConfiguration; } }
namespace Aws { namespace S3 { class S3Client; } }

namespace nix {

class S3BinaryCacheStore : public BinaryCacheStore
{
private:

    std::string bucketName;

    ref<Aws::Client::ClientConfiguration> config;
    ref<Aws::S3::S3Client> client;

public:

    S3BinaryCacheStore(const StoreFactory & storeFactory,
        const Path & secretKeyFile, const Path & publicKeyFile,
        const std::string & bucketName);

    void init() override;

    struct Stats
    {
        std::atomic<uint64_t> put{0};
        std::atomic<uint64_t> putBytes{0};
        std::atomic<uint64_t> putTimeMs{0};
        std::atomic<uint64_t> get{0};
        std::atomic<uint64_t> getBytes{0};
        std::atomic<uint64_t> getTimeMs{0};
        std::atomic<uint64_t> head{0};
    };

    const Stats & getS3Stats();

    bool isValidPath(const Path & storePath) override;

private:

    Stats stats;

    ref<Aws::Client::ClientConfiguration> makeConfig();

protected:

    bool fileExists(const std::string & path) override;

    void upsertFile(const std::string & path, const std::string & data) override;

    std::string getFile(const std::string & path) override;

};

}
