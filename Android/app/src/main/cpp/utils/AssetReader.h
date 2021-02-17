#ifndef ASSETREADER_H
#define ASSETREADER_H

#include <string>
#include <android/asset_manager.h>

class AssetReader {
public:
    AssetReader(const std::string &path, AAssetManager* assetManager) {
        m_path = path;
        m_asset = assetManager;
    }

    ~AssetReader() {
        if (m_data) {
            delete m_data;
            m_data = nullptr;
        }
    }

    void run() {
        AAsset *assetVertex = AAssetManager_open(m_asset, m_path.c_str(), AASSET_MODE_STREAMING);
        assert(assetVertex);
        m_size = AAsset_getLength(assetVertex);
        assert(m_size > 0);
        m_data = new char[m_size];
        AAsset_read(assetVertex, m_data, m_size);
        AAsset_close(assetVertex);
    }

    char *getOutData() { return m_data; }

    size_t getSize() { return m_size; }

private:
    std::string m_path;
    AAssetManager *m_asset = nullptr;
    char *m_data = nullptr;
    size_t m_size = 0;
};

#endif