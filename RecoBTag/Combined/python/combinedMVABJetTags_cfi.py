import FWCore.ParameterSet.Config as cms

combinedMVABJetTags = cms.EDProducer("JetTagProducer",
	jetTagComputer = cms.string('combinedMVAComputer'),
	tagInfos = cms.VInputTag(
		cms.InputTag("impactParameterTagInfos"),
		cms.InputTag("inclusiveSecondaryVertexFinderTagInfos"),
		cms.InputTag("softPFMuonsTagInfos"),
		cms.InputTag("softPFElectronsTagInfos")
	)
)
