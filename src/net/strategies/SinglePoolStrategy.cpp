/* XMRig
 * Copyright 2010      Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2012-2014 pooler      <pooler@litecoinpool.org>
 * Copyright 2014      Lucas Jones <https://github.com/lucasjones>
 * Copyright 2014-2016 Wolf9466    <https://github.com/OhGodAPet>
 * Copyright 2016      Jay D Dee   <jayddee246@gmail.com>
 * Copyright 2016-2017 XMRig       <support@xmrig.com>
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#include "interfaces/IStrategyListener.h"
#include "net/Client.h"
#include "net/strategies/SinglePoolStrategy.h"
#include "Options.h"


SinglePoolStrategy::SinglePoolStrategy(const Url *url, const char *agent, IStrategyListener *listener) :
    m_active(false),
    m_listener(listener)
{
    m_client = new Client(0, agent, this);
    m_client->setUrl(url);
    m_client->setRetryPause(Options::i()->retryPause() * 1000);
}


bool SinglePoolStrategy::isActive() const
{
    return m_active;
}


void SinglePoolStrategy::connect()
{
    m_client->connect();
}


void SinglePoolStrategy::submit(const JobResult &result)
{
    m_client->submit(result);
}


void SinglePoolStrategy::onClose(Client *client, int failures)
{
    if (!isActive()) {
        return;
    }

    m_active = false;
    m_listener->onPause(this);
}


void SinglePoolStrategy::onJobReceived(Client *client, const Job &job)
{
    m_listener->onJob(client, job);
}


void SinglePoolStrategy::onLoginSuccess(Client *client)
{
    m_active = true;
    m_listener->onActive(client);
}
