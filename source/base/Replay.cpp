#include "Replay.hpp"

#include <cstring>
#include <stdexcept>

using namespace mines;

void Replay::Record(ReplayEntry::Action action, Index row, Index col, double timestamp)
{
    entries_.push_back({action, row, col, timestamp});
}

void Replay::Clear() noexcept
{
    entries_.clear();
}

bool Replay::Save(const std::filesystem::path &path) const
{
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out) return false;

    // Header
    out.write(Magic, 4);
    out.write(reinterpret_cast<const char*>(&Version), 1);

    auto count = static_cast<uint32_t>(entries_.size());
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const auto &e : entries_)
    {
        out.write(reinterpret_cast<const char*>(&e.action),    sizeof(e.action));
        out.write(reinterpret_cast<const char*>(&e.row),       sizeof(e.row));
        out.write(reinterpret_cast<const char*>(&e.col),       sizeof(e.col));
        out.write(reinterpret_cast<const char*>(&e.timestamp), sizeof(e.timestamp));
    }

    return out.good();
}

Replay Replay::Load(const std::filesystem::path &path)
{
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("Cannot open replay file: " + path.string());

    char magic[4];
    in.read(magic, 4);
    if (std::memcmp(magic, Magic, 4) != 0)
        throw std::runtime_error("Invalid replay file format");

    uint8_t version;
    in.read(reinterpret_cast<char*>(&version), 1);
    if (version != Version)
        throw std::runtime_error("Unsupported replay version");

    uint32_t count;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));

    Replay replay;
    replay.entries_.reserve(count);

    for (uint32_t i = 0; i < count; ++i)
    {
        ReplayEntry e;
        in.read(reinterpret_cast<char*>(&e.action),    sizeof(e.action));
        in.read(reinterpret_cast<char*>(&e.row),       sizeof(e.row));
        in.read(reinterpret_cast<char*>(&e.col),       sizeof(e.col));
        in.read(reinterpret_cast<char*>(&e.timestamp), sizeof(e.timestamp));
        replay.entries_.push_back(e);
    }

    if (!in) throw std::runtime_error("Replay file truncated");
    return replay;
}

std::filesystem::path Replay::DefaultPath()
{
    return std::filesystem::path("replays") / "last.mswr";
}
