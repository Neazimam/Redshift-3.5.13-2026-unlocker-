#ifndef PATCHER_H
#define PATCHER_H

#include <string>
#include <vector>

// Low-level worker function
void patch_binary(const std::string& path, const std::vector<int>& search, const std::vector<unsigned char>& replace);

// High-level wrapper to call from your button
void run_redgiant_activation();

#endif
