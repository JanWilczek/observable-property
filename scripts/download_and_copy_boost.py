import hashlib
import os
import shutil
import tarfile
import urllib.request

# Constants
BOOST_URL = "https://archives.boost.io/release/1.86.0/source/boost_1_86_0.tar.gz"
BOOST_SHA256 = "2575e74ffc3ef1cd0babac2c1ee8bdb5782a0ee672b1912da40e5b4b591ca01f"
TARGET_DIR = "include/observable_property/detail/boost"
BOOST_ARCHIVE = "boost_1_86_0.tar.gz"
BOOST_SRC_DIR = "boost_1_86_0"

def download_boost():
    print(f"Downloading Boost from {BOOST_URL}...")
    urllib.request.urlretrieve(BOOST_URL, BOOST_ARCHIVE)
    print("Download complete.")

def verify_sha256(filename, expected_sha256):
    print("Verifying SHA256 checksum...")
    sha256_hash = hashlib.sha256()
    with open(filename, "rb") as f:
        for byte_block in iter(lambda: f.read(4096), b""):
            sha256_hash.update(byte_block)
    calculated_sha256 = sha256_hash.hexdigest()
    if calculated_sha256 != expected_sha256:
        raise ValueError("SHA256 checksum does not match. File may be corrupted.")
    print("SHA256 checksum verified.")

def extract_boost():
    print("Extracting Boost archive...")
    with tarfile.open(BOOST_ARCHIVE, "r:gz") as tar:
        tar.extractall()
    print("Extraction complete.")

def copy_boost_module(module_name: str):
    source_path = os.path.join(BOOST_SRC_DIR, "boost", module_name)
    target_path = os.path.join(TARGET_DIR, module_name)
    print(f"Copying {module_name} directory from {source_path} to {target_path}...")
    if not os.path.exists(target_path):
        os.makedirs(target_path)
    shutil.copytree(source_path, target_path, dirs_exist_ok=True)

    if module_name not in ["core", "mpl", "predef", "tuple", "exception", "detail", "move", "container_hash"]:
        copy_boost_header(f"{module_name}.hpp")

    print("Copy complete.")

def copy_boost_header(header_name: str):
    source_path = os.path.join(BOOST_SRC_DIR, "boost", header_name)
    target_path = TARGET_DIR
    print(f"Copying {header_name} from {source_path} to {target_path}...")
    if not os.path.exists(target_path):
        os.makedirs(target_path)
    shutil.copy(source_path, target_path)
    print("Copy complete.")

def copy_boost_modules():
    copy_boost_module("signals2")

    # signals2 dependencies
    copy_boost_module("assert")
    copy_boost_module("bind")
    copy_boost_module("config")
    copy_boost_module("core")
    copy_boost_module("function")
    copy_boost_module("iterator")
    copy_boost_module("mpl")
    copy_boost_module("optional")
    copy_boost_module("parameter")
    copy_boost_module("predef")
    copy_boost_module("preprocessor")
    copy_boost_module("smart_ptr")
    copy_boost_header("throw_exception.hpp")
    copy_boost_module("tuple")
    copy_boost_module("type_traits")
    copy_boost_module("variant")

    copy_boost_header("shared_ptr.hpp")
    copy_boost_header("cstdint.hpp")
    copy_boost_module("exception")
    copy_boost_header("version.hpp")
    copy_boost_module("detail")
    copy_boost_header("static_assert.hpp")
    copy_boost_module("move")
    copy_boost_header("type.hpp")
    copy_boost_header("none.hpp")
    copy_boost_header("none_t.hpp")
    copy_boost_module("utility")
    copy_boost_header("function_equal.hpp")
    copy_boost_header("mem_fn.hpp")
    copy_boost_header("get_pointer.hpp")
    copy_boost_header("is_placeholder.hpp")
    copy_boost_header("visit_each.hpp")
    copy_boost_header("weak_ptr.hpp")
    copy_boost_header("scoped_ptr.hpp")
    copy_boost_header("blank_fwd.hpp")
    copy_boost_module("type_index")
    copy_boost_module("container_hash")
    copy_boost_header("call_traits.hpp")
    copy_boost_module("functional")
    copy_boost_header("blank.hpp")
    copy_boost_module("integer")
    copy_boost_header("integer_fwd.hpp")
    copy_boost_header("limits.hpp")
    copy_boost_module("mp11")
    
    # Windows
    copy_boost_module("preprocessor")

def cleanup():
    print("Cleaning up...")
    os.remove(BOOST_ARCHIVE)
    shutil.rmtree(BOOST_SRC_DIR)
    print("Cleanup complete.")

if __name__ == "__main__":
    try:
        download_boost()
        verify_sha256(BOOST_ARCHIVE, BOOST_SHA256)
        extract_boost()
        copy_boost_modules()
        cleanup()
        print("Boost library download and setup completed successfully.")
    except Exception as e:
        print(f"Error: {e}")

