/** @file  SampleMetaSvc.h
 *  @brief TL::SampleMetaSvc class header
 *  @class TL::SampleMetaSvc
 *  @brief C++11 singleton class for handling sample meta data
 *
 *  A utility for connect a DSID with meta data, which includes the
 *  initial state, the generator, and the type (nominal or
 *  systematic).
 *
 *  @author Douglas Davis, <ddavis@cern.ch>
 */

#ifndef TL_SampleMetaSvc_h
#define TL_SampleMetaSvc_h

// C++
#include <fstream>
#include <map>
#include <tuple>

// TL
#include <TopLoop/Core/Loggable.h>

namespace TL {

  /// @addtogroup Enums convenience enums
  /// @brief Enums to help with meta data
  /// @{

  enum class kCampaign {
    Unknown, Data, MC15c, MC16a, MC16c, MC16d, MC16e, MC16f
  };

  enum class kInitialState {
    Unknown  , Data     , ttbar   , tW       ,
    tW_DR    , tW_DS    , ttt     , tttt     ,
    Zjets    , Wjets    , WW      , WZ       ,
    ZZ       , Diboson  , ttbarZ  , ttbarW   ,
    ttbarll  , ttH      , tchan   , schan
  };

  enum class kGenerator {
    Unknown           , Data              , PowhegPythia6     , PowhegPythia8       ,
    PowhegHerwig      , PowhegHerwigpp    , Sherpa21          , Sherpa22            ,
    Sherpa221         , MadgraphPythia    , MadgraphPythia8   , aMCatNLOPythia8     ,
    aMCatNLOHerwig    , aMCatNLOHerwigpp  , PowhegPythia8_dil , PowhegPythia6_dil   ,
    PowhegHerwig7     , PowhegHerwig7_dil , Sherpa222         , aMCatNLOPythia8_dil ,
    PowhegPythia8_had
  };

  enum class kSampleType {
    Unknown, Data, Nominal, Systematic
  };

  enum class kSgTopNtup {
    Unknown, v23, v25, v27
  };

  /// @}

}

namespace TL {
  class SampleMetaSvc final : public TL::Loggable {
  private:

    SampleMetaSvc();
    ~SampleMetaSvc();

    SampleMetaSvc(const SampleMetaSvc&) = delete;
    SampleMetaSvc& operator=(const SampleMetaSvc&) = delete;
    SampleMetaSvc(SampleMetaSvc&&) = delete;
    SampleMetaSvc& operator=(SampleMetaSvc&&) = delete;

    std::map<std::string,TL::kSampleType>   m_s2e_ST;
    std::map<TL::kSampleType,std::string>   m_e2s_ST;

    std::map<std::string,TL::kInitialState> m_s2e_IS;
    std::map<TL::kInitialState,std::string> m_e2s_IS;

    std::map<std::string,TL::kGenerator>    m_s2e_G;
    std::map<TL::kGenerator,std::string>    m_e2s_G;

    std::map<std::string,TL::kCampaign>     m_s2e_C;
    std::map<TL::kCampaign,std::string>     m_e2s_C;

    std::map<std::string,TL::kSgTopNtup>    m_s2e_NT;
    std::map<TL::kSgTopNtup,std::string>    m_e2s_NT;

    std::map<std::string,TL::kCampaign>     m_rTags;

    TL::kSgTopNtup m_ntupVersion{TL::kSgTopNtup::Unknown};
    std::map<TL::kSgTopNtup,
             std::map<TL::kCampaign,float>> m_campaignLumis;

    void setupMaps();

    typedef std::map<int,std::tuple<TL::kInitialState,TL::kGenerator,TL::kSampleType>> SampleTable_t;
    SampleTable_t m_sampleTable;

    const SampleTable_t::const_iterator checkTable(const unsigned int dsid) const {
      const SampleTable_t::const_iterator itr = m_sampleTable.find(dsid);
      if ( itr == m_sampleTable.end() ) {
        logger()->error("can't find DSID! {} not in SampleMetaSvc table!",dsid);
      }
      return itr;
    }

    template <typename Enumeration>
    auto as_integer(const Enumeration value) const -> typename std::underlying_type<Enumeration>::type {
      return static_cast<typename std::underlying_type<Enumeration>::type>(value);
    }

  public:

    /// get the instance of the singleton class
    /**
     *  The instance can be used using:
     *  @code{.cpp}
     *  TL::kInitialState initstate = TL::SampleMetaSvc::get().getInitialState(410015);
     *  std::string initstatestr    = TL::SampleMetaSvc::get().getInitialStateStr(410015);
     *  @endcode
     */
    static SampleMetaSvc& get() {
      static SampleMetaSvc inst;
      return inst;
    }

    /// @name Enum and string identification getters taking DSID
    /// @{

    /// retrieve a enum value corresponding to the initial state based on a DSID
    TL::kInitialState getInitialState(const unsigned int dsid) const {
      auto itr = checkTable(dsid);
      return std::get<0>(itr->second);
    }
    /// retrieve a enum value corresponding to the generator based on a DSID
    TL::kGenerator    getGenerator(const unsigned int dsid)    const {
      auto itr = checkTable(dsid);
      return std::get<1>(itr->second);
    }
    /// retrieve a enum value corresponding to the sample type based on a DSID
    TL::kSampleType   getSampleType(const unsigned int dsid)   const {
      auto itr = checkTable(dsid);
      return std::get<2>(itr->second);
    }

    /// get the initial state name based on a dsid
    const std::string getInitialStateStr(const unsigned int dsid) const {
      return as_string(getInitialState(dsid));
    }

    /// get the generator name based on a dsid
    const std::string getGeneratorStr(const unsigned int dsid) const {
      return as_string(getGenerator(dsid));
    }

    /// get the sample type name based on a dsid
    const std::string getSampleTypeStr(const unsigned int dsid) const {
      return as_string(getSampleType(dsid));
    }

    /// @}

  public:
    /// @name Campaign helper functions
    /// @{

    /// Given a sample name, get the MC campaign identifier
    TL::kCampaign getCampaign(const std::string& sample_name) const;

    /// given the campaign enum entry get the string
    const std::string getCampaignStr(const TL::kCampaign campaign) const {
      auto itr = m_e2s_C.find(campaign);
      if ( itr == m_e2s_C.end() ) {
        logger()->error("can't find campaign enum entry");
      }
      return itr->second;
    }

    /// Given a sample name, get the MC campaign string identifier
    /**
     *  @param sample_name string which should be the rucio sample
     *  name.
     *  @param log_it flag (default true) to print result at "info"
     *  level.
     */
    const std::string getCampaignStr(const std::string& sample_name, bool log_it) const {
      auto retval = as_string(getCampaign(sample_name));
      if  ( log_it ) {
        logger()->info("This appears to be campaign: {}",retval);
      }
      return retval;
    }

    /// get the luminosity of a particular campaign
    /**
     *  Lumi values associated with campaigns are defined in
     *  `TopLoop/data/campaigns.json`
     *
     *  @param campaign the campaign of interest.
     */
    float getLumi(const TL::kCampaign campaign) const;

    /// Given a list of campaigns, get the total data luminosity associated with them
    /**
     *  During runtime we can retrieve the luminosities defined in the
     *  file `TopLoop/data/campaigns.json`.
     *
     *  Example usage:
     *  @code{.cpp}
     *  float a_c_lumi = TL::SampleMetaSvc::get().getCampaignLumi({TL::kCampaign::MC16a,TL::kCampaign::MC16c});
     *  @endcode
     *
     *  @param campaigns list of campaigns.
     *
     */
    float getLumi(const std::vector<TL::kCampaign>& campaigns) const;

    /// generate a campaign weight based on the campaign enum entry
    /**
     *  If you are using multiple campaigns to create an expectation
     *  from MC, you'll need to handle the luminosities
     *  carefully. This function lets you generate a weight such that
     *  in the future you can scale all samples to the same total
     *  luminosity. This weight will be valid in combination with the
     *  samples that you feed to the second argument of this function.
     *
     *  The weight that is returned is calculated as:
     *
     *  \f[
     *     w_c = \frac{\mathcal{L}_c}{\sum_i \mathcal{L}_i}
     *  \f]
     *
     *  where \f$\mathcal{L}_c\f$ is the luminosity associated with
     *  the campaign you are generating a weight for and
     *  \f$\{\mathcal{L}_i\}\f$ is the set of campaign luminosities.
     *
     *  @param campaign the campaign to generate the weight for
     *  @param campaigns the list of campaigns you want to be compatible with
     */
    float getCampaignWeight(const TL::kCampaign campaign,
                            const std::vector<TL::kCampaign>& campaigns) const;

    /// generate a campaign weight based on the rucio dataset name
    /**
     *  Use the rucio dataset name to generate a weight based on the
     *  given list of campaigns. See the other version of
     *  getCampaignWeight() for more details.
     *
     *  Example usage for MC16c weight based for use in combination with MC16a:
     *  @code{.cpp}
     *  std::string datasetname = "some string which contains 'r9781'"
     *  float campWeight = TL::SampleMetaSvc::get().getCampaignWeight(datasetname,
     *                                                                {TL::kCampaign::MC16a,
     *                                                                 TL::kCampaign::MC16c});
     *  @endcode
     *  @param rucioDir the string for the dataset name
     *  @param campaigns the list of campaigns you want to be compatible with
     */
    float getCampaignWeight(const std::string& rucioDir,
                            const std::vector<TL::kCampaign>& campaigns) const;

    /// get the year associated with a run number in data
    /**
     *  As defined from the ATLAS DQ GRLs.
     *  GRL info retrieved from links on this AtlasProtected TWiki:
     *  TopDerivationMC16aList
     *
     *  @param runNum the run number
     */
    unsigned int getYear(const unsigned int runNum) const;

    /// @}

    /// @name Misc helper functions
    /// @{

    /// given a sample name, return if the sample was simulated with AFII
    /**
     *  @param sample_name string which should be the rucio sample
     *  name.
     *  @param log_it flag (default true) to print result at "info"
     *  level.
     */
    bool isAFII(const std::string& sample_name, bool log_it = true) const;

    /// Determine if the DSID is PowPy8 ttbar or Wt
    /**
     *  Cleans up some user code
     *
     *  @param d the DSID
     */
    bool tWorTtbarPowPy8(const unsigned int d) const;

    /// set the single top ntuple version (controls campaign lumis)
    /**
     *  The class uses ntuple version information internally to make
     *  lumi calculations based on campaigns and ntuple,
     *  versions. This function manually sets the internal ntuple
     *  version used... if getNtupleVersion is called the information
     *  is overriden!
     */
    void setNtupleVersionForCampaignUse(const TL::kSgTopNtup ntupVersion) {
      m_ntupVersion = ntupVersion;
    }

    /// given the sample name, get the SgTop ntuple version
    /**
     *  This function will make this class internally use the
     *  determined ntuple version for luminosity information!
     */
    TL::kSgTopNtup getNtupleVersion(const std::string& sample_name, bool log_it = true);

    /// convenience function to grab which ntuple version is in use by this class
    std::string ntupleVersionInUse() const {
      return as_string(m_ntupVersion);
    }

    /// @}

    /// @name Printing utilties
    /// @{

    /// have the logger print out info based on dsid
    void printInfo(const int dsid) const;
    /// print all available sample metadata
    void dump();

    /// @}

  private:
    const std::string as_string(const TL::kInitialState ienum) const {
      auto itr = m_e2s_IS.find(ienum);
      if ( itr == m_e2s_IS.end() ) {
        logger()->error("can't find initial state enum entry");
      }
      return itr->second;
    }

    const std::string as_string(const TL::kGenerator ienum) const {
      auto itr = m_e2s_G.find(ienum);
      if ( itr == m_e2s_G.end() ) {
        logger()->error("can't find generator enum entry");
      }
      return itr->second;
    }

    const std::string as_string(const TL::kSampleType ienum) const {
      auto itr = m_e2s_ST.find(ienum);
      if ( itr == m_e2s_ST.end() ) {
        logger()->error("can't find sample type enum entry");
      }
      return itr->second;
    }

    const std::string as_string(const TL::kCampaign ienum) const {
      auto itr = m_e2s_C.find(ienum);
      if ( itr == m_e2s_C.end() ) {
        logger()->error("Can't find campaign enum entry");
      }
      return itr->second;
    }

    const std::string as_string(const TL::kSgTopNtup ienum) const {
      auto itr = m_e2s_NT.find(ienum);
      if ( itr == m_e2s_NT.end() ) {
        logger()->error("Can't find  single top ntuple version enum entry");
      }
      return itr->second;
    }

  };
}

#endif
