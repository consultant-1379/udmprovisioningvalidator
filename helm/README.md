# eric-udm-udmprovisioningvalidator

UDM provisioning validator microservice deployment

## Values

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| env.schema.path | string | `"/bin/UDMProvisioningValidation.yaml"` |  |
| global.activation.nodeSelector | object | `{}` |  |
| global.hpa.enabled | string | `"off"` |  |
| global.monitorResources.cpu.validator | string | `"main_cpu_request"` |  |
| global.monitorResources.volumePath | string | `"/etc/podinfo"` |  |
| global.overloadProtection.enabled | string | `"on"` |  |
| global.registry.imagePullPolicy | string | `"IfNotPresent"` |  |
| global.registry.pullSecret | string | `nil` |  |
| global.registry.url | string | `"armdocker.rnd.ericsson.se"` |  |
| global.resources.enabled | string | `"on"` |  |
| global.timezone | string | `"UTC"` |  |
| global.tolerations.enabled | string | `"on"` |  |
| global.tolerations.seconds | int | `300` |  |
| global.updateStrategy.rollingUpdate.maxSurge | int | `1` |  |
| global.updateStrategy.rollingUpdate.maxUnavailable | string | `"50%"` |  |
| global.updateStrategy.type | string | `"RollingUpdate"` |  |
| hpa.replicas.max | int | `10` |  |
| hpa.replicas.min | int | `2` |  |
| hpa.target.cpu | int | `70` |  |
| imageCredentials.registry.pullSecret | string | `nil` |  |
| imageCredentials.repoPath | string | `"proj_hss/5g/docker_images"` |  |
| images.udmprovisioningvalidator.name | string | `"eric-ccdmeda-udmprovisioningvalidator"` |  |
| images.udmprovisioningvalidator.tag | string | `"do_not_change_to_be_updated_by_pipeline"` |  |
| readinessProbe.failureThreshold | int | `2` |  |
| readinessProbe.initialDelaySeconds | int | `5` |  |
| readinessProbe.periodSeconds | int | `3` |  |
| readinessProbe.timeoutSeconds | int | `2` |  |
| replicaCount | int | `2` |  |
| resources.healthproxy.limits.cpu | string | `"5m"` |  |
| resources.healthproxy.limits.memory | string | `"20Mi"` |  |
| resources.healthproxy.requests.cpu | string | `"5m"` |  |
| resources.healthproxy.requests.memory | string | `"20Mi"` |  |
| resources.main.limits.cpu | string | `"1000m"` |  |
| resources.main.limits.memory | string | `"100Mi"` |  |
| resources.main.requests.cpu | string | `"1000m"` |  |
| resources.main.requests.memory | string | `"50Mi"` |  |
| service.id | string | `"eric-udm-validation"` |  |
| service.name | string | `"eric-udm-udmprovisioningvalidator"` |  |
| service.port.http2 | int | `9002` |  |
| service.type | string | `"ClusterIP"` |  |
| sidecars.healthproxy.images.healthproxy.name | string | `"eric-udm-healthproxy"` |  |
| sidecars.healthproxy.images.healthproxy.tag | string | `"1.12.1"` |  |
| sidecars.healthproxy.port.http | int | `9400` |  |
| sidecars.healthproxy.service.name | string | `"healthproxy"` |  |
