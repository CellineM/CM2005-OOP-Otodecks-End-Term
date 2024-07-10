/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   vinyldisc_jpg;
    const int            vinyldisc_jpgSize = 6120559;

    extern const char*   stopbutton_png;
    const int            stopbutton_pngSize = 9199;

    extern const char*   uploading_png;
    const int            uploading_pngSize = 11753;

    extern const char*   playbutton_png;
    const int            playbutton_pngSize = 13525;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 4;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
