function(create_resources directory output)
    file(WRITE ${output} "#include <stdint.h>\n\n")
    file(GLOB bins "${directory}/*.png") # Add other file types if necessary

    foreach(bin ${bins})
        string(REGEX MATCH "([^/]+)$" filename ${bin})
        string(REGEX REPLACE "\.[^.]*$" "" filename ${filename})
        string(REGEX REPLACE "\\.| |-" "_" filename ${filename})
        string(TOLOWER ${filename} filename)

        file(READ ${bin} filedata HEX)
        string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1, " filedata ${filedata})
        string(REGEX REPLACE "..$" "" filedata ${filedata})

        file(APPEND ${output} "const uint8_t ${filename}[] = { ${filedata} };\n")
        file(APPEND ${output} "const uint64_t ${filename}_size = sizeof(${filename});\n\n")
    endforeach()
endfunction()