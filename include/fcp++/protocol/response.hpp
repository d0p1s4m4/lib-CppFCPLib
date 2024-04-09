#ifndef FPC_PROTOCOL_RESPONSE_HPP_
#define FPC_PROTOCOL_RESPONSE_HPP_

#include <string>
namespace fcp::protocol {

class Response
{
public:
  enum class Type
  {
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

    /**
     * Node answer to \ref Request::ClientHello
     *
     * \code{.unparsed}
     * NodeHello
     * CompressionCodecs=4 - GZIP(0), BZIP2(1), LZMA(2), LZMA_NEW(3)
     * Revision=build01475
     * Testnet=false
     * Version=Fred,0.7,1.0,1475
     * Build=1475
     * ConnectionIdentifier=6f467be43d838f8e02877e7f176a73bd
     * Node=Fred
     * ExtBuild=29
     * FCPVersion=2.0
     * NodeLanguage=ENGLISH
     * ExtRevision=v29
     * EndMessage
     * \endcode
     */
  struct NodeHello
  {
    /** FCP protocol version, must be 2.0 */
    std::string FCPVersion;
    std::string Node;
    std::string Version;
    bool Testnet;
    std::string Identifier;
    std::string NodeLanguage;
  };
};

}

#endif // !FPC_PROTOCOL_RESPONSE_HPP_