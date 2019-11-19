//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////

#include "FWCore/Catalog/interface/InputFileCatalog.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Catalog/interface/SiteLocalConfig.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include <boost/algorithm/string.hpp>

#include <iostream>

namespace edm {

  InputFileCatalog::InputFileCatalog(std::vector<std::string> const& fileNames,
                                     std::string const& override,
                                     bool useLFNasPFNifLFNnotFound)
      : logicalFileNames_(fileNames),
        fileNames_(fileNames),
        fallbackFileNames_(fileNames.size()),
        fileCatalogItems_(),
        fileLocator_(),
        overrideFileLocator_(),
        fallbackFileLocator_(),
        overrideFallbackFileLocator_() {
    init(override, "", useLFNasPFNifLFNnotFound);
  }
  
  InputFileCatalog::InputFileCatalog(std::vector<std::string> const& fileNames,
                                     std::string const& override,
                                     std::string const& overrideFallback,
                                     bool useLFNasPFNifLFNnotFound)
      : logicalFileNames_(fileNames),
        fileNames_(fileNames),
        fallbackFileNames_(fileNames.size()),
        fileCatalogItems_(),
        fileLocator_(),
        overrideFileLocator_(),
        fallbackFileLocator_(),
        overrideFallbackFileLocator_() {
        //HERE
        for (unsigned int i = 0 ; i < fileNames.size() ; ++i) {
          std::cout << "\n Filename " << i << fileNames[i] ;
        }
    init(override, overrideFallback, useLFNasPFNifLFNnotFound);
  }
  
  //HERE
  /*
  InputFileCatalog::InputFileCatalog(std::vector<std::string> const& fileNames,
                                     std::string const& override,
                                     bool useLFNasPFNifLFNnotFound, bool dummy)
      : logicalFileNames_(fileNames),
        fileNames_(fileNames),
        fallbackFileNames_(fileNames.size()),
        fileCatalogItems_(),
        fileLocator_(),
        overrideFileLocator_(),
        fallbackFileLocator_(),
        overrideFallbackFileLocator_() {
    init(override, useLFNasPFNifLFNnotFound);
  }*/


  InputFileCatalog::~InputFileCatalog() {}

  void InputFileCatalog::init(std::string const& inputOverride,
                              std::string const& inputOverrideFallback,
                              bool useLFNasPFNifLFNnotFound) {
    
    fileCatalogItems_.reserve(fileNames_.size());
    typedef std::vector<std::string>::iterator iter;
    for (iter it = fileNames_.begin(),
              lt = logicalFileNames_.begin(),
              itEnd = fileNames_.end(),
              ft = fallbackFileNames_.begin();
         it != itEnd;
         ++it, ++lt, ++ft) {
      boost::trim(*it);
      
      //HERE
      std::string pfnTmp = *it ;
      std::string lfnTmp = *lt ;

      if (it->empty()) {
        throw Exception(errors::Configuration, "InputFileCatalog::InputFileCatalog()\n")
            << "An empty string specified in the fileNames parameter for input source.\n";
      }
      if (isPhysical(*it)) {
        if (it->back() == ':') {
          throw Exception(errors::Configuration, "InputFileCatalog::InputFileCatalog()\n")
              << "An empty physical file name specified in the fileNames parameter for input source.\n";
        }
        // Clear the LFN.
        lt->clear();
        //HERE
        lfnTmp = *lt ;
      } else {
        if (!fileLocator_) {
          fileLocator_ = std::make_unique<FileLocator>("", false);  // propagate_const<T> has no reset() function
        }
        if (!overrideFileLocator_ && !inputOverride.empty()) {
          overrideFileLocator_ =
              std::make_unique<FileLocator>(inputOverride, false);  // propagate_const<T> has no reset() function
        }
        if (!fallbackFileLocator_) {
          try {
            fallbackFileLocator_ =
                std::make_unique<FileLocator>("", true);  // propagate_const<T> has no reset() function
          } catch (cms::Exception const& e) {
            // No valid fallback locator is OK too.
          }
        }
        if (!overrideFallbackFileLocator_ && !inputOverrideFallback.empty()) {
          overrideFallbackFileLocator_ =
              std::make_unique<FileLocator>(inputOverrideFallback, true);  // propagate_const<T> has no reset() function
        }
         
        boost::trim(*lt);
        lfnTmp = *lt ; 
        findFile(*it, *ft, *lt, useLFNasPFNifLFNnotFound); //this convert file name provided in cfg to full file name 
      }
      fileCatalogItems_.push_back(FileCatalogItem(*it, *lt, *ft, pfnTmp, lfnTmp));
    }
    
    //HERE build other file locators from local config service which are used to read file using other data catalog 
    Service<SiteLocalConfig> localconfservice ;
    std::vector<std::string> tmp_dataCatalogs = localconfservice->otherDataCatalogs() ;
    for (unsigned int iTmp = 0 ; iTmp < tmp_dataCatalogs.size() ; ++iTmp) {
      std::cout << "\n Constructing file locator " << tmp_dataCatalogs[iTmp] << std::endl ;
      try {
        otherFileLocators_.push_back(std::make_unique<FileLocator>(tmp_dataCatalogs[iTmp], false)) ;
      } catch (cms::Exception const& e) {
        continue ;
      }
    }

  }
  

  void InputFileCatalog::findFile(std::string& pfn,
                                  std::string& fallbackPfn,
                                  std::string const& lfn,
                                  bool useLFNasPFNifLFNnotFound) {
    if (overrideFileLocator_) {
      pfn = overrideFileLocator_->pfn(lfn);
      if (pfn.empty()) {
        pfn = fileLocator_->pfn(lfn);
      }
    } else {
      pfn = fileLocator_->pfn(lfn);
    }
    if (pfn.empty() && useLFNasPFNifLFNnotFound) {
      pfn = lfn;
    }
    // Empty PFN will be found by caller.

    if (overrideFallbackFileLocator_) {
      fallbackPfn = overrideFallbackFileLocator_->pfn(lfn);
      if (fallbackFileLocator_ && fallbackPfn.empty()) {
        fallbackPfn = fallbackFileLocator_->pfn(lfn);
      }
    } else if (fallbackFileLocator_) {
      fallbackPfn = fallbackFileLocator_->pfn(lfn);
      // Empty fallback PFN is OK.
    }
  }
}  // namespace edm
