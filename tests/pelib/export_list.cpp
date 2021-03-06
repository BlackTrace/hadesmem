// Copyright (C) 2010-2015 Joshua Boyce
// See the file COPYING for copying permission.

#include <hadesmem/pelib/export_list.hpp>
#include <hadesmem/pelib/export_list.hpp>

#include <sstream>
#include <utility>

#include <hadesmem/detail/warning_disable_prefix.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <hadesmem/detail/warning_disable_suffix.hpp>

#include <hadesmem/config.hpp>
#include <hadesmem/error.hpp>
#include <hadesmem/module.hpp>
#include <hadesmem/module_list.hpp>
#include <hadesmem/pelib/export.hpp>
#include <hadesmem/pelib/export_dir.hpp>
#include <hadesmem/pelib/pe_file.hpp>
#include <hadesmem/process.hpp>
#include <hadesmem/read.hpp>

// Export something to ensure tests pass...
extern "C" __declspec(dllexport) void Dummy();
extern "C" __declspec(dllexport) void Dummy()
{
}

void TestExportList()
{
  hadesmem::Process const process(::GetCurrentProcessId());

  hadesmem::PeFile pe_file_1(
    process, ::GetModuleHandleW(nullptr), hadesmem::PeFileType::kImage, 0);

  bool processed_one_export_list = false;

  hadesmem::ModuleList modules(process);
  for (auto const& mod : modules)
  {
    hadesmem::PeFile const cur_pe_file(
      process, mod.GetHandle(), hadesmem::PeFileType::kImage, 0);

    hadesmem::ExportList cur_export_list(process, cur_pe_file);
    if (std::begin(cur_export_list) == std::end(cur_export_list))
    {
      continue;
    }

    hadesmem::ExportDir cur_export_dir(process, cur_pe_file);

    processed_one_export_list = true;

    for (auto const& e : cur_export_list)
    {
      hadesmem::Export const test_export(
        process, cur_pe_file, e.GetProcedureNumber());

      if (test_export.ByName())
      {
        BOOST_TEST(!test_export.GetName().empty());
      }
      else
      {
        BOOST_TEST(test_export.ByOrdinal());
        BOOST_TEST(test_export.GetProcedureNumber() >=
                   cur_export_dir.GetOrdinalBase());
      }

      if (test_export.IsForwarded())
      {
        BOOST_TEST(!test_export.GetForwarder().empty());
        BOOST_TEST(!test_export.GetForwarderModule().empty());
        BOOST_TEST(!test_export.GetForwarderFunction().empty());
      }
      else
      {
        BOOST_TEST(test_export.GetRva() != 0);
        BOOST_TEST(test_export.GetVa() != nullptr);
      }

      std::stringstream test_str_1;
      test_str_1.imbue(std::locale::classic());
      test_str_1 << e;
      std::stringstream test_str_2;
      test_str_2.imbue(std::locale::classic());
      test_str_2 << e.GetProcedureNumber();
      BOOST_TEST_EQ(test_str_1.str(), test_str_2.str());
    }
  }

  BOOST_TEST(processed_one_export_list);
}

int main()
{
  TestExportList();
  return boost::report_errors();
}
