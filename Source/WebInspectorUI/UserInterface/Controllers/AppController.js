/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

WI.AppController = class AppController extends WI.AppControllerBase
{
    constructor()
    {
        super();

        this._hasExtraDomains = false;
        this._debuggableType = InspectorFrontendHost.debuggableType() === "web" ? WI.DebuggableType.Web : WI.DebuggableType.JavaScript;
    }

    // Properties.

    get hasExtraDomains() { return this._hasExtraDomains; }
    get debuggableType() { return this._debuggableType; }

    // API.

    activateExtraDomains(domains)
    {
        if (this._hasExtraDomains)
            throw new Error("Extra domains have already been activated, cannot activate again.");

        this._hasExtraDomains = true;

        for (let domain of domains) {
            let agent = InspectorBackend.activateDomain(domain);
            if (agent.enable)
                agent.enable();
        }

        // FIXME: all code within WI.activateExtraDomains should be distributed elsewhere.
        WI.activateExtraDomains();
    }
};
