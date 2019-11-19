#ifndef FWCore_Catalog_InputFileCatalog_h
#define FWCore_Catalog_InputFileCatalog_h
//////////////////////////////////////////////////////////////////////
//
// Class InputFileCatalog. Services to manage InputFile catalog
//
//////////////////////////////////////////////////////////////////////

#include <memory>
#include <string>
#include <vector>
#include "FWCore/Catalog/interface/FileLocator.h"
#include "FWCore/Utilities/interface/propagate_const.h"

namespace edm {
  class FileCatalogItem {
  public:
    FileCatalogItem() : pfn_(), lfn_(), fallbackPfn_() {}
    FileCatalogItem(std::string const& pfn, std::string const& lfn, std::string const& fallbackPfn)
        : pfn_(pfn), lfn_(lfn), fallbackPfn_(fallbackPfn) {}
    //HERE
    FileCatalogItem(std::string const& pfn, std::string const& lfn, std::string const& fallbackPfn, std::string const& rawPfn, std::string const& rawLfn)
        : pfn_(pfn), lfn_(lfn), fallbackPfn_(fallbackPfn), rawPfn_(rawPfn), rawLfn_(rawLfn) {}
    std::string const& fileName() const { return pfn_; }
    std::string const& logicalFileName() const { return lfn_; }
    std::string const& fallbackFileName() const { return fallbackPfn_; }
    //HERE
    std::string const& rawPfn() const { return rawPfn_; }
    std::string const& rawLfn() const { return rawLfn_; }


  private:
    std::string pfn_;
    std::string lfn_;
    std::string fallbackPfn_;
    //HERE
    std::string rawPfn_ ;
    std::string rawLfn_ ;
  };

  class InputFileCatalog {
  public:
    InputFileCatalog(std::vector<std::string> const& fileNames,
                     std::string const& override,
                     bool useLFNasPFNifLFNnotFound = false);
    InputFileCatalog(std::vector<std::string> const& fileNames,
                     std::string const& override,
                     std::string const& overrideFallback,
                     bool useLFNasPFNifLFNnotFound = false);
    InputFileCatalog(std::vector<std::string> const& fileNames,
                     std::string const& override,
                     std::string const& overrideFallback,
                     bool useLFNasPFNifLFNnotFound = false,
                     bool dummy = true);

    ~InputFileCatalog();
    std::vector<FileCatalogItem> const& fileCatalogItems() const { return fileCatalogItems_; }
    std::vector<std::string> const& logicalFileNames() const { return logicalFileNames_; }
    std::vector<std::string> const& fileNames() const { return fileNames_; }
    std::vector<std::string> const& fallbackFileNames() const { return fallbackFileNames_; }
    bool empty() const { return fileCatalogItems_.empty(); }
    static bool isPhysical(std::string const& name) { return (name.empty() || name.find(':') != std::string::npos); }

  private:
    //HERE
    //void init(std::string const& override, bool useLFNasPFNifLFNnotFound);
    void init(std::string const& override, std::string const& overrideFallback, bool useLFNasPFNifLFNnotFound);
    void findFile(std::string& pfn, std::string& fallbackPfn, std::string const& lfn, bool useLFNasPFNifLFNnotFound);
    std::vector<std::string> logicalFileNames_;
    std::vector<std::string> fileNames_;
    std::vector<std::string> fallbackFileNames_;
    std::vector<FileCatalogItem> fileCatalogItems_;
    edm::propagate_const<std::unique_ptr<FileLocator>> fileLocator_;
    edm::propagate_const<std::unique_ptr<FileLocator>> overrideFileLocator_;
    edm::propagate_const<std::unique_ptr<FileLocator>> fallbackFileLocator_;
    edm::propagate_const<std::unique_ptr<FileLocator>> overrideFallbackFileLocator_;
    //HERE
    //std::vector<edm::propagate_const<std::unique_ptr<FileLocator>> > fileLocators_;
  };
}  // namespace edm

#endif
