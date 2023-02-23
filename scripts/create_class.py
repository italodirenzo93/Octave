from sys import argv
from os import path

class_name = argv[1]
if not class_name:
    raise "No class name provided. Exiting..."

class_path = path.split(class_name)
class_name = class_path[-1]

# Header
header_filename = f"{class_name}.hpp"

header_code = f"""#ifndef OCTAVE_{class_name.upper()}_HPP
#define OCTAVE_{class_name.upper()}_HPP

namespace Octave {{

class {class_name} {{
public:
\t// Constructors
\t{class_name}() = default;
\t{class_name}( const {class_name}& other ) = default;
\t{class_name}( {class_name}&& other ) noexcept = default;
\t~{class_name}() noexcept = default;

\t// Operators
\t{class_name}& operator=( const {class_name}& other ) = default;
\t{class_name}& operator=( {class_name}&& other ) noexcept = default;

\t// Getter/setter pairs

\t// Methods

\t// Fields

protected:
\t// Methods

\t// Fields

private:
\t// Methods

\t// Fields

}};

}}

#endif

"""

# Source
source_filename = f"{class_name}.cpp"

source_code = f"""#include "pch/pch.hpp"
#include "{header_filename}"

namespace Octave {{

// Code...

}}

"""

# Write files
project_root = "./"
path_parts = ("src", *class_path[:1])

header_loc = path.join(project_root, *path_parts, header_filename)
with open(path.realpath(header_loc), 'w') as header_file:
    header_file.write(header_code)

source_loc = path.join(project_root, *path_parts, source_filename)
with open(path.realpath(source_loc), 'w') as source_file:
    source_file.write(source_code)

print(f"Created header file at {path.join(header_loc)}.")
print(f"Created source file at {path.join(source_loc)}.")
