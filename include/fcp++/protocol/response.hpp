#ifndef FPC_PROTOCOL_RESPONSE_HPP_
#define FPC_PROTOCOL_RESPONSE_HPP_

namespace fcp::protocol {

class Response {
    public:
    enum {
        NodeHello,
        CloseConnectionDuplicateClientName,
        Peer,
        PeerNode,
        EndListPeers,
        EndListPeerNotes,
        PeerRemoved,
        NodeData,
        ConfigDate,
        TestDDAReply,
        TestDDAComplete,
        SSKKeyPair,
        PersistentGet,
        PersistentPut,
        PersistentPutDir,
        URIGenerated,
        PutSuccessful,
        PutFetchable,
        DataFound,
        GetRequestStatus,
        AllData,
        StartedCompression,
        FinishedCompression,
        SimpleProgress,
        ExpectedHashes,
        ExpectedMIME,
        ExpectedDataLength,
        CompatibilityMode,
        EndListPersistantRequest,
        PersistentRequestRemoved,
        PersistantRequestModified,
        SendingToNetwork,
        EnterFiniteCooldown,
        GenerateMetadata,
        PutFailed,
        GetFailed,
        ProtocolError,
        IdentifierCollision,
        UnknownNoneIdentifier,
        UnknownPerrNoteType,
        SubscribedUSK,
        SubscribedUSKUpdate,
        SubscribedUSKSendingToNetwork,
        SubscribedUSKRoundFinished,
        PluginInfo,
        PluginRemoved,
        FCPPluginReply,
        ProbeBandwidth,
        ProbeBuild,
        ProbeError,
        ProbeIdentifier,
        ProbeLinkLengths,
        ProbeLocation,
        ProbeRefused,
        ProbeRejectStats,
        ProbeStoreSize,
        ProbeUptime
    };
};

}

#endif // !FPC_PROTOCOL_RESPONSE_HPP_