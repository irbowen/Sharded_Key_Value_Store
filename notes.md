
    // Send msg if not type noaction

    // If client_req msg
    // skip

    // Incoming: prepare
    // Arguments: n
    // Acting as: Acceptor
    // Check if you've seen an n higher than this one
    // Outgoing: a prepare_accept
    //    OR prepare_reject

    // Incoming: prepare_accept
    // Arguments: n_a, v_a (could be null)
    // Acting as: Proposer
    // Increment the count of # of prepare accepts you have gotten
    // Outgoing: If this is more than a majority, send propose_value(n, v)

    // Incoming: prepare_reject
    // Arguments: n_p
    // Acting as: Proposer
    // If we got a higher n value, update our internal storage with this new value
    // Outgoing:

    // Incoming: propose
    // Arguments: n, v
    // Acting as: Acceptor
    // Outgoing: propose_accept(n) + broadcast_to_learners(n, v),
    //    OR propose_reject(n)

    // Incoming: propose_accept
    // Arguments: n
    // Acting as: Proposer
    // Outgoing:

    // Incoming: propose_reject
    // Arguments: n_p
    // Acting as: Proposer

    // Incoming: broadcast_to_learners
    // Arguments: n, v
    // Acting as: Learner
    // Outgoing: potenial client response
