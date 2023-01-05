#!/usr/bin/env pwsh
# Copyright 2018-2019, Bingxing Wang <uefi-oss-projects@imbushuo.net>
# All rights reserved.

# Check current commit ID and write it into file for SMBIOS reference. (Trim it)
# Check current date and write it into file for SMBIOS reference too. (MM/dd/yyyy)
Write-Output "Stamp build."
# This one is EDK2 base commit
Set-Location MU_BASECORE
$edk2Commit = git rev-parse HEAD
# This is SurfaceDuoPkg package commit
Set-Location ..
$commit = git rev-parse HEAD
$date = (Get-Date).Date.ToString("MM/dd/yyyy")
$user = (whoami)
try {
    $machine = [System.Net.Dns]::GetHostByName((hostname)).HostName
} catch {
    $machine = "$(hostname)"
}
$owner = "$($user)@$($machine)"
if ($null -eq $machine) { $owner = $user }

if ($commit) {
    $commit = $commit.Substring(0, 8)
    $edk2Commit = $edk2Commit.Substring(0, 8)

    $releaseInfoContent = @(
        "#ifndef __SMBIOS_RELEASE_INFO_H__",
        "#define __SMBIOS_RELEASE_INFO_H__",
        "#ifdef __IMPL_COMMIT_ID__",
        "#undef __IMPL_COMMIT_ID__",
        "#endif",
        "#define __IMPL_COMMIT_ID__ `"$($commit)`"",
        "#ifdef __RELEASE_DATE__",
        "#undef __RELEASE_DATE__",
        "#endif",
        "#define __RELEASE_DATE__ `"$($date)`"",
        "#ifdef __BUILD_OWNER__",
        "#undef __BUILD_OWNER__",
        "#endif",
        "#define __BUILD_OWNER__ `"$($owner)`"",
        "#ifdef __EDK2_RELEASE__",
        "#undef __EDK2_RELEASE__",
        "#endif",
        "#define __EDK2_RELEASE__ `"$($edk2Commit)`"",
        "#endif"
    )

    Set-Content -Path Platforms/viliPkg/Include/Resources/ReleaseInfo.h -Value $releaseInfoContent -ErrorAction SilentlyContinue -Force
}
